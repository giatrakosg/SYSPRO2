//
//  Reader.cpp
//  SYSPRO2
//
//  Created by <author> on 01/04/2019.
//
//

#include "Reader.hpp"

Reader::Reader(int buff,int fromID,int toID,char *sDir,char *cDir) : buffer_size(buff) ,
from(fromID) , to(toID) , count(MAX_TRIES){
    outDir = new char [strlen(sDir) + 1];
    strcpy(outDir,sDir);
    commonDir = new char [strlen(cDir) + 1];
    strcpy(commonDir,cDir);

}
// The skeleton of the implementation is taken from
// https://www.tutorialspoint.com/inter_process_communication/inter_process_communication_named_pipes.htm
// and adapted to the protocol specified

int Reader::createPipe(void) {

    fifo_file = new char[MAX_NAME];
    sprintf(fifo_file,"%s/%d_to_%d.fifo",commonDir,from,to);

    /* Create the FIFO if it does not exist */
    mkfifo(fifo_file,0666);
    pipeD = open(fifo_file, O_RDONLY );
    return 0;
}
int Reader::readFromPipe(void) {
    int read_bytes ;
    while (1) {
        short titleLen; // We get the 2 bytes for the name length
        read_bytes = read(pipeD,&titleLen,2);
        if (titleLen == 0) {
            break;
        }
        printf("READER:Reading file with %d size of title \n",titleLen);
        char *title = new char[titleLen + 1];
        read_bytes = read(pipeD,title,titleLen);
        title[titleLen] = '\0';
        char path[256];
        sprintf(path,"%s/%s",outDir,title);
        FILE * outD = fopen(path,"r+");
        printf("READER:Reading file with title %s\n",title);
        short f_size ;
        read_bytes = read(pipeD,&f_size,2);
        printf("READER:Reading file with %d size\n",f_size);
        char *contents = new char[f_size + 1];
        memset(contents,0,f_size+1);
        while (f_size > 0) {
            char *readBuffer = new char[buffer_size+1];
            memset(readBuffer,'\0',buffer_size+1);
            read_bytes = read(pipeD,readBuffer,buffer_size);
            strcat(contents,readBuffer);

            f_size -= read_bytes ;
        }
        fprintf(stdout,"%s",contents );
        fclose(outD);
    }

    return 0 ;
}
Reader::~Reader() {
    close(pipeD);
    delete outDir ;
    delete commonDir ;
}
