//
//  Client.cpp
//  SYSPRO2
//
//  Created by <author> on 27/03/2019.
//
//

#include "Client.hpp"
// Taken from http://www.anyexample.com/programming/c/qsort__sorting_array_of_strings__integers_and_structs.xml
int myStrCmp(const void *str1,const void *str2) {
    if ((str1 == NULL) || (str2 == NULL)) {
        printf("NULL string \n");
    }
    return strcmp((char *)str1, (char *)str2);
}

int Client::getIDfromString(char *string) {
    // We get the id as an integer from the string eg. 1.id etc
    char *out = new char[strlen(string) + 1];
    for (int i = 0; i < (int)strlen(string); i++) {
        if (string[i] == '.') {
            break ;
        }
        out[i] = string[i];

    }
    int retID = atoi(out);
    delete out ;
    return retID;
}

Client::Client():last_seen(0){}
// Parses arguments given in cmd line and sets variables
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
                input_dir = new char[strlen(optarg) + 1];
                strcpy(input_dir,optarg);
                break;
            case 'm':
                mirror_dir = new char[strlen(optarg) + 1];
                strcpy(mirror_dir,optarg);
                break;
            case 'b':
                buff_size = atoi(optarg);
                break;
            case 'l' :
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
// Open the files / directories and do parameter checking
int Client::parseArgs(void) {
    // Check if the common dir exists and create it if it doesnt
    struct  stat cs = {0};
    if (stat(common_dir,&cs) == -1) {
        mkdir(common_dir,0755);
    }
    c_dir = opendir(common_dir);

    // Check if the input dir exists and exit it if it doesnt
    struct  stat is = {0};
    if (stat(input_dir,&is) == -1) {
        return -1 ;
    }
    i_dir = opendir(input_dir);

    // Check if the mirror dir exists , if it doesnt create it
    m_dir = opendir(mirror_dir);
    if (m_dir == NULL) {
        mkdir(mirror_dir,0755);
    }
    // Create log file if it doesnt exit and open
    log = fopen(log_file,"w+");
    if (log == NULL) {
        perror("fopen");
        return -1 ;
    }

    return 0 ;
}
// Function that prints arguments .Used for debugging
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

// Creates the .id file and writes the process id
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
    sprintf(id_fn,"%s/%s",common_dir,id_s);
    f_id = fopen(id_fn,"w");
    if (f_id == NULL) {
        return -1 ;
    }
    long ppid = (long) getpid() ;
    fprintf(f_id, "%ld",ppid);
    fclose(f_id);
    // We lock the log file to write that we created a client
    flock(fileno(log),LOCK_EX);
    fprintf(log, "client %d\n",id );
    flock(fileno(log),LOCK_UN);
    return 0 ;

}
// Function that cycles through contents of common dir and compares it to the
// previous instance .Updates the copy stored at seen .
int Client::detectNewID(void) {
    struct dirent *ind;
    char newContents[SEEN_BUFFER][FILE_NAME];
    rewinddir(c_dir);

    // We cycle through the directory and save all contents on a sorted array
    int i = 0 ;
    while ((ind = readdir(c_dir)) != NULL) {
        if ((strcmp(ind->d_name,".") == 0) || (strcmp(ind->d_name,"..") == 0) ||
            (strstr(ind->d_name,".fifo") != NULL)) {
            continue ;
        }
        strcpy(newContents[i],ind->d_name);
        i++;
    }
    // Sort the array using qsort for bsearching later
    qsort(newContents,i,FILE_NAME,myStrCmp);

    // Check if any new .id files have apeared in the common dir
    for (int j = 0; j < i; j++) {
        char *item = (char *)bsearch(newContents[j],seen,last_seen,FILE_NAME,myStrCmp);
        if (item != NULL) {
            continue ;
        } else  {
            // A new id was detected .Write it to log and spawn the processes
            flock(fileno(log),LOCK_EX);
            fprintf(log, "detected %s\n",newContents[j] );
            flock(fileno(log),LOCK_UN);
            int to = getIDfromString(newContents[j]);
            createProcesses(to,0);
        }
    }
    // Check if any files in the previous instance have vanished .
    // If yes then call system delete local copy
    for (int j = 0; j < last_seen; j++) {
        char *item = (char *)bsearch(seen[j],newContents,i,FILE_NAME,myStrCmp);
        if (item == NULL) {
            flock(fileno(log),LOCK_EX);
            fprintf(log, "Deleted %s\n",seen[j] );
            flock(fileno(log),LOCK_UN);

            int id = getIDfromString(seen[j]);
            char cmd[256] ;
            // Delete the deleted clients dir from mirror dir
            sprintf(cmd,"rm -r %s/%d",mirror_dir,id);
            system(cmd);
        }
    }

    // Update the seen buffer with newContents
    for (int j = 0; j < i; j++) {
        strcpy(seen[j],newContents[j]);
    }
    last_seen = i ;
    return 0 ;

}
int Client::createProcesses(int to,int retries) {
    pid_t rdpid = createReaderProcess(to);
    pid_t wrpid = createWriterProcess(to);
    list.add(to,rdpid,wrpid,retries);
    // We add the 2 pids with it id to the list
    return 0 ;
}
// Calls waitpid to all the procesess in pidlist
// Thus freeing them
int Client::checkProcesses(void) {
    list.free();
    return 0 ;
}
// If we received a SIGUSR1 from the corresponding pid then we find
// the id in our list and restart the 2 processes
int Client::restartProcesses(void) {
    sigset_t mask;
    sigfillset(&mask);
    sigprocmask(SIG_SETMASK, &mask, NULL);
    if (failpid == -1) {
        return -1;
    }
    printf("Restart proc %ld\n",(long) failpid );
    struct pidentry *m = list.find(failpid);
    if (m == NULL) {
        fprintf(stderr, "Error : pid not found ");
        return -1 ;
    }
    if (m->retries == 3) {
        fprintf(stderr, "Error : pid already retried 3 times ");
        return -1 ;
    }
    // Kill both the reader and the writer
    kill(m->writer,SIGKILL);
    kill(m->reader,SIGKILL);
    // Update retries and recall processes
    int retries = m->retries ;
    retries++;
    int rid = m->id ;
    list.remove(rid);
    createProcesses(rid,retries);
    failpid = -1 ;
    sigprocmask(SIG_UNBLOCK, &mask, NULL);
    return 0 ;

}
pid_t Client::createReaderProcess(int to) {
    char toID[5] = {0};
    sprintf(toID,"%d",to);
    char fromID[5] = {0};
    sprintf(fromID,"%d",id);
    char buff_string[10];
    sprintf(buff_string,"%d",buff_size);
    pid_t child = fork() ;
    if (child == 0) {
        // We are in the child
        execl("./reader_client","reader_client",buff_string,toID,fromID,mirror_dir,common_dir,log_file,(char *)NULL);
        perror("exec");
        return -1 ;
    }
    return child ;
}
pid_t Client::createWriterProcess(int to) {
    char toID[5] = {0};
    sprintf(toID,"%d",to);
    char fromID[5] = {0};
    sprintf(fromID,"%d",id);
    char buff_string[10];
    sprintf(buff_string,"%d",buff_size);
    pid_t child = fork() ;
    if (child == 0) {
        // We are in the child
        execl("./writer_client","writer_client",buff_string,fromID,toID,input_dir,common_dir,log_file,(char *)NULL);
        perror("exec");
        return -1 ;
    }
    return child ;
}
Client::~Client() {
    std::cout <<  "Deleting Client Object " << std::endl ;
    closedir(i_dir);
    closedir(c_dir);
    closedir(m_dir);
    char cmd[1024] ;
    sprintf(cmd,"rm -r %s/*",mirror_dir);
    system(cmd);
    perror("syscall");
    fclose(log);
    remove(id_fn);
    delete id_fn ;
    delete id_s ;
    delete common_dir ;
    delete input_dir ;
    delete mirror_dir ;
    delete log_file ;
}
