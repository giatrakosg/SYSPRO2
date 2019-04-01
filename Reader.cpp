//
//  Reader.cpp
//  SYSPRO2
//
//  Created by <author> on 01/04/2019.
//
//

#include "Reader.hpp"

Reader::Reader(int fromID,int toID,char *sDir,char *cDir) :
from(fromID) , to(toID){
    toDir = new char [strlen(sDir) + 1];
    strcpy(toDir,sDir);
    commonDir = new char [strlen(cDir) + 1];
    strcpy(commonDir,sDir);

}
Reader::~Reader() {
    delete toDir ;
    delete commonDir ;
}
