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


class Reader {
private:
    static void alarmhandler(int );
    int count ;
    int from ;
    int to ;
    char *toDir;
    char *commonDir ;
    int fd ; // File descriptor used to open and read from .fifo file
public:
    Reader(int ,int ,char *,char *); // Id from , id to
    // to directory , common dir
    int createPipe(void);
    ~Reader();
protected:

};


#endif /* Reader_hpp */
