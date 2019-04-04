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
    char titleLen[3]; // We get the 2 bytes for the name length
    read_bytes = read(pipeD,titleLen,2);
    titleLen[2] = '\0' ;
    int titleSize = atoi(titleLen);
    //printf("READER:Reading file with %d size of title \n",titleSize);
    char *title = new char[titleSize + 1];
    read_bytes = read(pipeD,title,titleSize);
    title[titleSize] = '\0';
    //printf("READER:Reading file with title %s\n",title);
    char fileSize[3] = {0} ;
    short f_size ;
    read_bytes = read(pipeD,&f_size,2);
    //printf("READER:Reading file with %d size\n",f_size);
    char *contents = new char[f_size + 1];
    memset(contents,0,f_size+1);
    char readBuffer[buffer_size+1];
    while (f_size > 0) {
        char readBuffer[buffer_size+1] = {'\0'};
        read_bytes = read(pipeD,readBuffer,buffer_size);
        strcat(contents,readBuffer);

        f_size -= read_bytes ;
    }
    printf("%s",contents );

    return 0 ;
}
Reader::~Reader() {
    close(pipeD);
    delete outDir ;
    delete commonDir ;
}
