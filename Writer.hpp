//
//  Writer.hpp
//  SYSPRO2
//
//  Created by <author> on 01/04/2019.
//
//

#ifndef Writer_hpp
#define Writer_hpp

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <libgen.h>
#include <dirent.h>
#include <stdint.h>
#include <sys/file.h>


class Writer {
private:
    int buff ;
    int from ;
    int to ;
    char *inp_dir ;
    char *common_dir ;
    char *log_file ;
    FILE *logF ;
    DIR * idirPtr;

    int pipeD ; // File descriptor for fifo
    int sendFile(char *,char *); // Send file in this path
    int sendFilesInDir(char *dirpath);
public:
    Writer(int ,int ,int ,char *,char *,char *);// From id,to id , input dir, common dir
    int connect(void) ; // Connects to the .fifo
    int sendFiles(void); // Sends all files in inp_dir

    ~Writer();
protected:

};


#endif /* Writer_hpp */
