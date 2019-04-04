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
    mkfifo(fifo_file, 0666);
    pipeD = open(fifo_file, O_WRONLY);
    return 0 ;

}
int Writer::sendFile(char *path) {
    struct stat st;
    stat(path, &st);
    short fSize = st.st_size;
    printf("%d\n",fSize );
    int fd = open(path,O_RDONLY);
    char *fName = basename(path) ;
    char fLen[3] = {0};
    sprintf(fLen,"%d",strlen(fName));
    write(pipeD,fLen,2);
    write(pipeD,fName,strlen(fName));
    char fS[3] = {0};
    write(pipeD,&fSize,2);

    char contents[buff+1];
    while(fSize > 0) {
        read(fd,contents,buff);
        write(pipeD,contents,buff);
        fSize -= buff ;

    }
    close(pipeD);
}
Writer::~Writer() {
    delete common_dir ;
    delete inp_dir ;
}
