//
//  Writer.cpp
//  SYSPRO2
//
//  Created by <author> on 01/04/2019.
//
//

#include "Writer.hpp"

Writer::Writer(int buff,int from,int to,char *inpDir,char *cDir,char *logfile) : buff(buff) ,
from(from) , to(to) {
    inp_dir = new char[strlen(inpDir) + 1];
    strcpy(inp_dir,inpDir);
    common_dir = new char[strlen(cDir) + 1];
    strcpy(common_dir,cDir);
    log_file = new char[strlen(logfile) + 1];
    strcpy(log_file,logfile);
    logF = fopen(log_file,"w");
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
int Writer::sendFilesInDir(char *dirpath) {
    DIR * dir = opendir(dirpath);
    struct dirent *ind ;
    while((ind = readdir(dir)) != NULL) {
        char path[512];
        fprintf(logF,"%s\n",ind->d_name );
        sprintf(path,"%s/%s",dirpath,ind->d_name);
        // We check if it is a dir or a regular file
        // This code snippet was taken from
        // https://stackoverflow.com/questions/4553012/checking-if-a-file-is-a-directory-or-just-a-file
        struct stat path_stat;
        stat(path, &path_stat);
        if(!S_ISREG(path_stat.st_mode)) {
            continue ;
        }
        fprintf(logF,"%s\n",path );
        fflush(stdout);
        // We check if it
        sendFile(path);
    }
    closedir(dir);
    return 0 ;
}
int Writer::sendFiles(void) {
    sendFilesInDir(inp_dir);
    struct dirent *ind ;
    while ((ind = readdir(idirPtr)) != NULL) {
        char path[512];
        sprintf(path,"./%s/%s",inp_dir,ind->d_name);
        // We check if it is a dir or a regular file
        // This code snippet was taken from
        // https://stackoverflow.com/questions/4553012/checking-if-a-file-is-a-directory-or-just-a-file
        struct stat path_stat;
        stat(path, &path_stat);
        if(!S_ISREG(path_stat.st_mode)) {
            if ((strcmp(ind->d_name,".") == 0) || (strcmp(ind->d_name,"..") == 0)) {
                continue ;
            }
            sprintf(path,"%s/",path);
            fprintf(logF,"Dir : %s\n",path );
            sendFilesInDir(path);
        }
    }
    close(pipeD);
    short end = 0 ;
    write(pipeD,&end,2);

}
Writer::~Writer() {
    fclose(logF);
    closedir(idirPtr);
    delete common_dir ;
    delete inp_dir ;
}
