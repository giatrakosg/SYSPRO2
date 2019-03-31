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
#include <getopt.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/inotify.h>

#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define EVENT_BUF_LEN    ( 1024 * ( EVENT_SIZE + 16 ) )

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

    int cdir_nfy_d ; // Common dir file descriptor used by
    // inotify
    int wd ; // Watch list descriptor
public:
    Client();
    void getArgs(int ,char **); // Parses cmd line arguments
    void printArgs(void); // Prints cmd line arguments
    int  parseArgs(void) ; // Initiliazes values
    int writeID(void); // Writes file with id in common dir
    int listen(void);
    int detectNewID(void); // Scans the common dir for new id
    // and places it in the ids array

    ~Client();
protected:

};


#endif /* Client_hpp */
