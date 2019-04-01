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
#include <iostream>
#include <string.h>

class Reader {
private:
    int from ;
    int to ;
    char *toDir;
    char *commonDir ;
public:
    Reader(int ,int ,char *,char *); // Id from , id to
    // to directory , common dir
    ~Reader();
protected:

};


#endif /* Reader_hpp */
