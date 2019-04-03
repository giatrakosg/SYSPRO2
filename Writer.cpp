//
//  Writer.cpp
//  SYSPRO2
//
//  Created by <author> on 01/04/2019.
//
//

#include "Writer.hpp"

Writer::Writer(int from,int to,char *inpDir,char *cDir) : from(from) , to(to) {
    inp_dir = new char[strlen(inpDir) + 1];
    strcpy(inp_dir,inpDir);
    common_dir = new char[strlen(cDir) + 1];
    strcpy(common_dir,cDir);

}
Writer::~Writer() {
    delete common_dir ;
    delete inp_dir ;
}
