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

class Client {
private:
    int id ;
    char *common_dir ;
    char *input_dir ;
    char *mirror_dir ;
    int buff_size ;
    char *log_file ;

    DIR* c_dir ; // Pointer to common directory
    DIR* i_dir ; // Pointer to input dir
    DIR* m_dir ; // Pointer to mirror dir
public:
    Client();
    void getArgs(int ,char **); // Parses cmd line arguments 
    void printArgs(void); // Prints cmd line arguments
    int  parseArgs(void) ; // Initiliazes values
    ~Client();
protected:

};


#endif /* Client_hpp */
