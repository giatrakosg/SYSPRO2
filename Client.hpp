//
//  Client.hpp
//  SYSPRO2
//
//  Created by <author> on 27/03/2019.
//
//

#ifndef Client_hpp
#define Client_hpp

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <signal.h>
#include <getopt.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <wait.h>
#include <unistd.h>
#include <sys/file.h>
#include <gpgme.h>

#define SEEN_BUFFER 256
#define FILE_NAME 256
class Client {
private:
    int id ;   // Int representation of id
    char* id_s ; // String with id
    char *common_dir ;
    char *input_dir ;
    char *mirror_dir ;
    int buff_size ;
    char *log_file ;

    DIR* c_dir ; // Pointer to common directory
    DIR* i_dir ; // Pointer to input dir
    DIR* m_dir ; // Pointer to mirror dir
    FILE* log  ; // Pointer to log file
    FILE* f_id ; // Pointer to file id
    char* id_fn ; // Filename of id

    char seen[SEEN_BUFFER][FILE_NAME] ; // Strings of files we have already seen
    int last_seen ;
    int createReaderProcess(int ); // To id
    int createWriterProcess(int );
    int getIDfromString(char *);

public:
    Client();
    void getArgs(int ,char **); // Parses cmd line arguments
    void printArgs(void); // Prints cmd line arguments
    int parseArgs(void) ; // Initiliazes values
    int writeID(void); // Writes file with id in common dir
    int detectNewID(void); // Scans the common dir for new id
    // and places it in the ids array
    ~Client();
protected:

};


#endif /* Client_hpp */
