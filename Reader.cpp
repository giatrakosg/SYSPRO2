//
//  Reader.cpp
//  SYSPRO2
//
//  Created by <author> on 01/04/2019.
//
//

#include "Reader.hpp"

Reader::Reader(int fromID,int toID,char *sDir,char *cDir) :
from(fromID) , to(toID) , count(MAX_TRIES){
    toDir = new char [strlen(sDir) + 1];
    strcpy(toDir,sDir);
    commonDir = new char [strlen(cDir) + 1];
    strcpy(commonDir,cDir);

}
void Reader::alarmhandler(int signum) {
    signal(SIGALRM,Reader::alarmhandler);
    printf("Just got an alarm \n");
    this->count--;
}

// The skeleton of the implementation is taken from
// https://www.tutorialspoint.com/inter_process_communication/inter_process_communication_named_pipes.htm
// and adapted to the protocol specified

int Reader::createPipe(void) {
    signal(SIGALRM,Reader::alarmhandler);

    char id1[5] ;
    sprintf(id1,"%d",from);
    char id2[5] ;
    sprintf(id2,"%d",to);

    char *fifo_file = new char[strlen(commonDir) + strlen(id1) +
    strlen("_to_") + strlen(id2) + 2];
    sprintf(fifo_file,"%s/%s_to_%s",commonDir,id1,id2);
    printf("%s\n",fifo_file );
    char titleLen[3]; // 2 bytes denoting the length of the title
    char *title ; // Where we store the title
    char fileLen[4] ; // 4 bytes denoting the length of the  file
    char *contents ; // The contents of the file

    int read_bytes;
    /* Create the FIFO if it does not exist */
    mkfifo(fifo_file, S_IFIFO|0640);
    fd = open(fifo_file, O_RDWR);

    while(count > 0) {
        alarm(3);
        read_bytes = read(fd, titleLen, sizeof(titleLen) - 1);
        titleLen[read_bytes] = '\0';
        printf("Length of file title is %d\n",atoi(titleLen) );
        title = new char[atoi(titleLen) + 1];
        read_bytes = read(fd, title, sizeof(title) - 1);
        title[read_bytes] = '\0';
        printf("Read title of file %s\n",title );

    }
    close(fd);
    return 0;

}
Reader::~Reader() {
    delete toDir ;
    delete commonDir ;
}
