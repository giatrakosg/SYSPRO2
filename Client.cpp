//
//  Client.cpp
//  SYSPRO2
//
//  Created by <author> on 27/03/2019.
//
//

#include "Client.hpp"

Client::Client()  {}
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
    struct  stat cs = {0};
    if (stat(common_dir,&cs) == -1) {
        mkdir(common_dir,0755);
    }
    c_dir = opendir(common_dir);

    struct  stat is = {0};
    if (stat(input_dir,&is) == -1) {
        mkdir(input_dir,0755);
    }

    i_dir = opendir(input_dir);

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
    f_id = fopen(id_fn,"w+");
    if (f_id == NULL) {
        return -1 ;
    }
    fprintf(f_id, "%ld",(long)getpid() );
    return 0 ;

}

int Client::listen(void) {
    getchar();
    return 0 ;
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
