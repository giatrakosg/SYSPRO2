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
                //puts ("option -a \n");
                //printf("With value %s\n",optarg );
                id = atoi(optarg);
                break;
            case 'c':
                //puts ("option -t \n");
                //printf("With value %s\n",optarg );
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
    c_dir = opendir(common_dir);
    if (c_dir == NULL) {
        perror("opendir");
        return -1 ;
    }
    i_dir = opendir(input_dir);
    if (i_dir == NULL) {
        perror("opendir");
        return -1 ;
    }
    m_dir = opendir(mirror_dir);
    if (m_dir == NULL) {
        perror("opendir");
        return -1 ;
    }
}
void Client::printArgs(void) {
    std::cout
    << "ID : " << id << std::endl
    << "C_DIR : " << common_dir << std::endl
    << "I_DIR : " << input_dir << std::endl
    << "M_DIR : " << mirror_dir << std::endl
    << "B_SIZE : " << buff_size << std::endl
    << "LOG_F : " << log_file << std::endl ;
}
Client::~Client() {}
