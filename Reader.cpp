//
//  Reader.cpp
//  SYSPRO2
//
//  Created by <author> on 01/04/2019.
//
//

#include "Reader.hpp"
// This function creates directories recursively
// Source : https://gist.github.com/JonathonReinhart/8c0d90191c38af2dcadb102c4e202950
int mkdir_p(const char *path)
{
    /* Adapted from http://stackoverflow.com/a/2336245/119527 */
    const size_t len = strlen(path);
    char _path[PATH_MAX];
    char *p;

    errno = 0;

    /* Copy string so its mutable */
    if (len > sizeof(_path)-1) {
        errno = ENAMETOOLONG;
        return -1;
    }
    strcpy(_path, path);

    /* Iterate the string */
    for (p = _path + 1; *p; p++) {
        if (*p == '/') {
            /* Temporarily truncate */
            *p = '\0';

            if (mkdir(_path, S_IRWXU) != 0) {}

            *p = '/';
        }
    }

    if (mkdir(_path, S_IRWXU) != 0) {
        if (errno != EEXIST)
            return -1;
    }

    return 0;
}


Reader::Reader(int buff,int fromID,int toID,char *sDir,char *cDir,char *logfile) : buffer_size(buff) ,
from(fromID) , to(toID) , count(MAX_TRIES){
    outDir = new char [strlen(sDir) + 1];
    strcpy(outDir,sDir);
    commonDir = new char [strlen(cDir) + 1];
    strcpy(commonDir,cDir);
    logF = fopen(logfile,"w");
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
        fprintf(logF,"READER:Reading file with %d size of title \n",titleLen);
        char *title = new char[titleLen + 1];
        read_bytes = read(pipeD,title,titleLen);
        title[titleLen] = '\0';

        // basename / dirname modify their argument .So we pass a copy
        char *title2 = new char[strlen(title) + 1];
        strcpy(title2,title);
        char *title3 = new char[strlen(title) + 1];
        strcpy(title3,title);

        // We retrieve the dir structure
        char basedir[256];
        strcpy(basedir,dirname(title2));
        char fileName[256] ;
        strcpy(fileName,basename(title3));

        // We add the output dir so that mkdir_p creates the directories in the
        // correct place and the id of the other user
        char dirstruct[512];
        sprintf(dirstruct,"%s/%d/%s/",outDir,from,basedir);
        mkdir_p(dirstruct);

        // Path to the copy of the file we are creating
        char path[512];
        sprintf(path,"%s%s",dirstruct,fileName);
        FILE * outD = fopen(path,"w+");

        fprintf(logF,"READER:Reading file with title %s\n",title);

        int f_size ;
        read_bytes = read(pipeD,&f_size,sizeof(int));
        fprintf(logF,"READER:Reading file with %d size\n",f_size);
        fflush(logF);

        char *contents = new char[f_size + 1];
        memset(contents,0,f_size+1);
        while (f_size > 0) {
            char *readBuffer = new char[buffer_size+1]; // We use a buffer to
            // read over the pipe
            memset(readBuffer,'\0',buffer_size+1);
            read_bytes = read(pipeD,readBuffer,buffer_size);
            strcat(contents,readBuffer);
            f_size -= read_bytes ;
            delete readBuffer;
        }
        fflush(logF);
        fprintf(outD,"%s",contents );
        fflush(outD);
        fclose(outD);
        delete title2 ;
        delete title3 ;
        delete contents ;
    }

    return 0 ;
}
Reader::~Reader() {
    fprintf(logF, "Exiting Reader \n");
    fclose(logF);
    close(pipeD);
    delete outDir ;
    delete commonDir ;
    delete fifo_file;
}
