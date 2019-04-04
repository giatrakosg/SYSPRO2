//
//  Client.cpp
//  SYSPRO2
//
//  Created by <author> on 27/03/2019.
//
//

#include "Client.hpp"
int Client::getIDfromString(char *string) {
    // We get the id as an integer from the string eg. 1.id etc
    char *out = new char[strlen(string) + 1];
    for (int i = 0; i < strlen(string); i++) {
        if (string[i] == '.') {
            break ;
        }
        out[i] = string[i];

    }
    printf("%d\n",atoi(out) );
    int retID = atoi(out);
    delete out ;
    return retID;
}

Client::Client(){}
void Client::getArgs(int argc,char **argv) {
    // Parse cmd line parameters using getopts
    // Modified example found at :
    // https://www.gnu.org/software/libc/manual/html_node/Getopt-Long-Option-Example.html#Getopt-Long-Option-Example
    int c ;
    while (1) {
        static struct option long_options[] =
        {
            /* These options don’t set a flag.
            We distinguish them by their indices. */
            {"id"         , required_argument , 0 , 'n'},
            {"common_dir" , required_argument , 0 , 'c'},
            {"input_dir"  , required_argument , 0 , 'i'},
            {"mirror_dir" , required_argument , 0 , 'm'},
            {"buffer_size", required_argument , 0 , 'b'},
            {"log_file"   , required_argument , 0 , 'l'},
            {0, 0, 0, 0}
        };

        /* getopt_long stores the option index here. */
        int option_index = 0;

        c = getopt_long (argc, argv, "n:c:i:m:b:l:",
        long_options, &option_index);

        /* Detect the end of the options. */
        if (c == -1)
            break;
        switch (c) {
            case 'n':
                id_s = new char[strlen(optarg) + 1 + 3];
                strcpy(id_s,optarg);
                strcat(id_s,".id");
                id = atoi(optarg);
                break;
            case 'c':
                common_dir = new char[strlen(optarg) + 1];
                strcpy(common_dir,optarg);
                break;
            case 'i':
                //puts ("option -v \n");
                //printf("With value %s\n",optarg );
                input_dir = new char[strlen(optarg) + 1];
                strcpy(input_dir,optarg);
                break;
            case 'm':
                //puts ("option --h1 \n");
                //printf("With value %s\n",optarg );
                mirror_dir = new char[strlen(optarg) + 1];
                strcpy(mirror_dir,optarg);
                break;
            case 'b':
                //puts ("option --h2 \n");
                //printf("With value %s\n",optarg );
                buff_size = atoi(optarg);
                break;
            case 'l' :
                //puts ("option -b \n");
                //printf("With value %s\n",optarg );
                log_file = new char[strlen(optarg) + 1];
                strcpy(log_file,optarg);
                break ;
            case '?':
            /* getopt_long already printed an error message. */
                break;
            default:
                abort ();
        }
    }
}
int Client::parseArgs(void) {
    // Check if the common dir exists and create it if it doesnt
    struct  stat cs = {0};
    if (stat(common_dir,&cs) == -1) {
        mkdir(common_dir,0755);
    }
    c_dir = opendir(common_dir);

    // Check if the input dir exists and create it if it doesnt

    struct  stat is = {0};
    if (stat(input_dir,&is) == -1) {
        mkdir(input_dir,0755);
    }

    i_dir = opendir(input_dir);

    // Check if the mirror dir exists , if it doesnt terminate
    m_dir = opendir(mirror_dir);
    if (m_dir == NULL) {
        perror("opendir");
        return -1 ;
    }

    log = fopen(log_file,"w+");
    if (log == NULL) {
        perror("fopen");
        return -1 ;
    }

    /*creating the INOTIFY instance*/
    cdir_nfy_d = inotify_init();

    /*checking for error*/
    if ( cdir_nfy_d < 0 ) {
      perror( "inotify_init" );
    }

    /*adding the “/tmp” directory into watch list. Here, the suggestion is to validate the existence of the directory before adding into monitoring list.*/
    wd = inotify_add_watch( cdir_nfy_d, common_dir , IN_CREATE | IN_DELETE );


    return 0 ;
}
void Client::printArgs(void) {
    std::cout
    << "ID : " << id << std::endl
    << "ID_S :" << id_s << std::endl
    << "C_DIR : " << common_dir << std::endl
    << "I_DIR : " << input_dir << std::endl
    << "M_DIR : " << mirror_dir << std::endl
    << "B_SIZE : " << buff_size << std::endl
    << "LOG_F : " << log_file << std::endl ;
}
int Client::writeID(void) {
    struct dirent *ind ;
    // We cycle through contents of c_dir for file with same id as this
    // mirror_client
    while ((ind = readdir(c_dir)) != NULL) {
        if (strcmp(ind->d_name,id_s) == 0) {
            fprintf(stderr, "Error : %s id already exists\n",id_s );
            return -1 ;
        }
    }
    id_fn = new char[strlen(common_dir) + strlen(id_s) + 1 + 2];
    strcpy(id_fn,common_dir);
    strcat(id_fn,"/");
    strcat(id_fn,id_s);
    f_id = fopen(id_fn,"w");
    if (f_id == NULL) {
        return -1 ;
    }
    std::cout << id_fn << std::endl ;
    long ppid = (long) getpid() ;
    fflush(f_id);
    fprintf(f_id, "%ld",ppid);
    fclose(f_id);
    return 0 ;

}

int Client::detectNewID(void) {

    std::cout << "Waiting for new id to appear " << std::endl ;
    char buffer[EVENT_BUF_LEN];
    int length = read( cdir_nfy_d, buffer, EVENT_BUF_LEN );

    /*checking for error*/
    if ( length < 0 ) {
      perror( "read" );
    }
    // Cycle through all events
    struct inotify_event *event ;
    for (int i = 0; i < length; i+= EVENT_SIZE + event->len) {
        event = (struct inotify_event *)&buffer[i];
        if ( event->len ) {
            if ( event->mask & IN_CREATE ) {
                printf( "New file %s created.\n", event->name );
                if (strstr(event->name,".fifo") != NULL) {
                    continue ; // A file with .fifo was created so we ignore and move on
                }
                int toID = getIDfromString(event->name);
                createReaderProcess(toID);
                createWriterProcess(toID);
            }
        }
        else if ( event->mask & IN_DELETE ) {
            printf( "File %s deleted.\n", event->name );
        }
    }
    return 0 ;

}
int Client::createReaderProcess(int to) {
    char toID[5] = {0};
    sprintf(toID,"%d",to);
    char fromID[5] = {0};
    sprintf(fromID,"%d",id);
    char buff_string[10];
    sprintf(buff_string,"%d",buff_size);

    printf("%s %s\n",toID,fromID );
    pid_t child = fork() ;
    if (child == 0) {
        // We are in the child
        execl("./reader_client","reader_client",buff_string,fromID,toID,mirror_dir,common_dir,(char *)NULL);
        perror("exec");
    }
}
int Client::createWriterProcess(int to) {
    char toID[5] = {0};
    sprintf(toID,"%d",to);
    char fromID[5] = {0};
    sprintf(fromID,"%d",id);
    char buff_string[10];
    sprintf(buff_string,"%d",buff_size);

    printf("%s %s\n",toID,fromID );
    pid_t child = fork() ;
    if (child == 0) {
        // We are in the child
        execl("./writer_client","writer_client",buff_string,fromID,toID,mirror_dir,common_dir,(char *)NULL);
        perror("exec");
    }

}
Client::~Client() {
    std::cout <<  "Deleting Client Object " << std::endl ;
    closedir(i_dir);
    closedir(c_dir);
    closedir(m_dir);
    fclose(log);
    fclose(f_id);
    remove(id_fn);
}
