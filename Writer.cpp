//
//  Writer.cpp
//  SYSPRO2
//
//  Created by <author> on 01/04/2019.
//
//

#include "Writer.hpp"
// We remove everything before the 1st /



Writer::Writer(int buff,int from,int to,char *inpDir,char *cDir,char *logfile) : buff(buff) ,
from(from) , to(to) {
    inp_dir = new char[strlen(inpDir) + 1];
    strcpy(inp_dir,inpDir);
    common_dir = new char[strlen(cDir) + 1];
    strcpy(common_dir,cDir);
    log_file = new char[strlen(logfile) + 1];
    strcpy(log_file,logfile);
    logF = fopen(log_file,"w+");
    idirPtr = opendir(inp_dir);

}
int Writer::connect(void) {
    char fifo_file[256] ;
    sprintf(fifo_file,"%s/%d_to_%d.fifo",common_dir,from,to);
    mkfifo(fifo_file, 0666);
    pipeD = open(fifo_file, O_WRONLY);
    return 0 ;

}
int Writer::sendFile(char *dir,char *name) {
    char *strippedpath = strtok(dir,"/");
    strippedpath = strtok(NULL,"");
/*
    char outPath[512];
    char inPath[512];
    sprintf(inPath,"./%s/%s",)
    if (strcmp(dir,input_dir) == 0) {
        sprintf(outPath,"./%s",name);
    } else {
        sprintf(outPath,"./%s/%s",dir,name);
    }


    struct stat st;
    stat(outpath, &st);
    short fSize = st.st_size;
    int fd = open(path,O_RDONLY);
    char *fName ;
    if ((strcmp(dir,".") == 0) ) {
        strcpy(fName,name);
    } else {
        sprintf(fName,"%s/%s",dir,name);
    }
    short tLen = (short)strlen(fName);
    fprintf(stdout,"File Name : %s , %hd\n",fName ,(short)strlen(fName));
    fflush(stdout);
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
*/
    fprintf(stdout, "Dir %s . Name %s \n",strippedpath,name );
    return 0 ;
}
int Writer::sendFilesInDir(char *dirpath) {
    DIR * dir = opendir(dirpath);
    if (dir == NULL) {
        fprintf(stderr, "NULL dir\n");
    }
    fprintf(stdout,"Dirpath sendFilesInDir : %s \n",dirpath);
    struct dirent *ind ;
    while((ind = readdir(dir)) != NULL) {
        fprintf(logF,"Dirpath : %s\n",ind->d_name );
        // We check if it is a dir or a regular file
        // This code snippet was taken from
        // https://stackoverflow.com/questions/4553012/checking-if-a-file-is-a-directory-or-just-a-file
        struct stat path_stat;
        char totalpath[512];
        sprintf(totalpath,"%s/%s",dirpath,ind->d_name);
        stat(totalpath, &path_stat);

        // Ignore . , .. directories
        if ((strcmp(ind->d_name,"..") == 0) || (strcmp(ind->d_name,".") == 0)) {
            continue ;
        }
        // This is a directory
        if(!S_ISREG(path_stat.st_mode)) {
            char path[512] ;
            sprintf(path,"%s/%s",dirpath,ind->d_name);
            sendFilesInDir(path);
            continue ;
        }
        fprintf(logF,"Path : %s\n",totalpath );
        //fflush(logF);
        // We check if it
        sendFile(dirpath,ind->d_name);
    }
    closedir(dir);
    return 0 ;
}
int Writer::sendFiles(void) {
    sendFilesInDir(inp_dir);
    /*
    struct dirent *ind ;
    while ((ind = readdir(idirPtr)) != NULL) {
        char path[512];
        sprintf(path,"%s/%s",inp_dir,ind->d_name);
        // We check if it is a dir or a regular file
        // This code snippet was taken from
        // https://stackoverflow.com/questions/4553012/checking-if-a-file-is-a-directory-or-just-a-file
        struct stat path_stat;
        stat(path, &path_stat);
        if(!S_ISREG(path_stat.st_mode)) {
            if ((strcmp(ind->d_name,".") == 0) || (strcmp(ind->d_name,"..") == 0)) {
                continue ;
            }
            fprintf(logF,"Dir : %s\n",path );
            sendFilesInDir(path);
        }
    }
    */
    short end = 0 ;
    write(pipeD,&end,2);

    close(pipeD);

}
Writer::~Writer() {
    fprintf(logF, "Exiting Writer \n");
    fclose(logF);
    closedir(idirPtr);
    delete common_dir ;
    delete inp_dir ;
}
