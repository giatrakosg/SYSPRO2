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
    idirPtr = opendir(inp_dir);

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
    int fd = open(path,O_RDONLY);
    char *fName = basename(path) ;
    short tLen = (short)strlen(fName);
    printf("File Name : %s , %hd\n",fName ,(short)strlen(fName));
    write(pipeD,&tLen,2);
    write(pipeD,fName,strlen(fName));
    write(pipeD,&fSize,2);
    while(fSize > 0) {
        char *contents = new char[buff+1];
        memset(contents,'\0',buff+1);
        read(fd,contents,buff);
        write(pipeD,contents,buff);
        fSize -= buff ;
    }
    close(fd);
    return 0 ;
}
int Writer::sendFiles(void) {
    struct dirent *ind ;
    printf("%s\n",inp_dir );
    while((ind = readdir(idirPtr)) != NULL) {
        char path[512];
        printf("%s\n",ind->d_name );
        sprintf(path,"./%s/%s",inp_dir,ind->d_name);
        // We check if it is a dir or a regular file
        // This code snippet was taken from
        // https://stackoverflow.com/questions/4553012/checking-if-a-file-is-a-directory-or-just-a-file
        struct stat path_stat;
        stat(path, &path_stat);
        if(!S_ISREG(path_stat.st_mode)) {
            continue ;
        }
        printf("%s\n",path );
        fflush(stdout);
        // We check if it
        sendFile(path);
    }
    write(pipeD,0,2);
    close(pipeD);
    return 0 ;
}
Writer::~Writer() {
    delete common_dir ;
    delete inp_dir ;
}
