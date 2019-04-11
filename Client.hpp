//
//  Client.hpp
//  SYSPRO2
//
//  Created by <author> on 27/03/2019.
//
//

#ifndef Client_hpp
#define Client_hpp

#include "pidlist.hpp"

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

#include <openssl/rand.h>
#include <openssl/rsa.h>
#include <openssl/engine.h>
#include <openssl/pem.h>
#include <openssl/bio.h>
#include <openssl/bn.h>


#define SEEN_BUFFER 256
#define FILE_NAME 512

extern bool runFlag ;
extern pid_t failpid ;

class Client {
private:
    int id ;   // Int representation of id
    char *id_s ; // String with id
    char *common_dir ;
    char *input_dir ;
    char *mirror_dir ;
    int   buff_size ;
    char *log_file ;

    DIR* c_dir ; // Pointer to common directory
    DIR* i_dir ; // Pointer to input dir
    DIR* m_dir ; // Pointer to mirror dir
    FILE* log  ; // Pointer to log file
    FILE* f_id ; // Pointer to file id
    char* id_fn ; // Filename of id

    pidlist list ;
    char seen[SEEN_BUFFER][FILE_NAME] ; // Strings of files we have already seen
    int last_seen ;
    pid_t createReaderProcess(int ); // To id
    pid_t createWriterProcess(int );
    int createProcesses(int );
    int getIDfromString(char *);

public:
    Client();
    void getArgs(int ,char **); // Parses cmd line arguments
    void printArgs(void); // Prints cmd line arguments
    int parseArgs(void) ; // Initiliazes values
    int writeID(void); // Writes file with id in common dir
    int detectNewID(void); // Scans the common dir for new id
    int checkProcesses(void);
    int restartProcesses(void);
    // and places it in the ids array
    ~Client();
protected:

};


#endif /* Client_hpp */
