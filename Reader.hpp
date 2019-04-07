//
//  Reader.hpp
//  SYSPRO2
//
//  Created by <author> on 01/04/2019.
//
//

#ifndef Reader_hpp
#define Reader_hpp

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

#define MAX_TRIES 3
#define MAX_NAME 2048

class Reader {
private:
    int buffer_size ; // Size of buffer to use over pipes
    int from ;
    int to ;
    int count ;
    char *outDir;
    char *commonDir ;
    char *fifo_file ; // Name of fifo file
    FILE *logF ;
    int pipeD ; // File descriptor used to open and read from .fifo file
public:
    Reader(int ,int ,int ,char *,char *,char *);
    // Buffer size Id from , id to
    // to directory , common dir
    int createPipe(void);
    int readFromPipe(void);
    ~Reader();
protected:

};


#endif /* Reader_hpp */
