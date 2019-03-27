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

class Client {
private:
    int id ;
    char *common_dir ;
    char *input_dir ;
    char *mirror_dir ;
    int buff_size ;
    char *log_file ;
public:
    Client();
    void getArgs(int ,char **);
    void printArgs(void);
    ~Client();
protected:

};


#endif /* Client_hpp */
