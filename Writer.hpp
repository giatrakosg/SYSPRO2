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

class Writer {
private:
    int from ;
    int to ;
    char *inp_dir ;
    char *common_dir ;
public:
    Writer(int ,int ,char *,char *);// From id,to id , input dir, common dir
    ~Writer();
protected:

};


#endif /* Writer_hpp */
