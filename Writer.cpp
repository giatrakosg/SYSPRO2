//
//  Writer.cpp
//  SYSPRO2
//
//  Created by <author> on 01/04/2019.
//
//

#include "Writer.hpp"

Writer::Writer(int buff,int from,int to,char *inpDir,char *cDir) : buff(buff) ,
from(from) , to(to) {
    inp_dir = new char[strlen(inpDir) + 1];
    strcpy(inp_dir,inpDir);
    common_dir = new char[strlen(cDir) + 1];
    strcpy(common_dir,cDir);

}
int Writer::connect(void) {
    char fifo_file[256] ;
    sprintf(fifo_file,"%s/%d_to_%d.fifo",common_dir,from,to);
    mkfifo(fifo_file, S_IFIFO|0666);
    pipeD = open(fifo_file, O_RDONLY);
    return 0 ;

}
int Writer::sendFile(char *path) {
    FILE *fd = fopen(path,"r");
    char *fName = basename(path) ;
    char fLen[3] = {0};
    sprintf(fLen,"%d",fName);
    write(pipeD,fLen,2);
}
Writer::~Writer() {
    delete common_dir ;
    delete inp_dir ;
}
