//
//  Writer.cpp
//  SYSPRO2
//
//  Created by <author> on 01/04/2019.
//
//

#include "Writer.hpp"
// We remove everything before the 1st /


// Allocate memory for the strings .Open the dirs/files(to append mode)
Writer::Writer(int buff,int from,int to,char *inpDir,char *cDir,char *logfile) : buff(buff) ,
from(from) , to(to) {
    inp_dir = new char[strlen(inpDir) + 1];
    strcpy(inp_dir,inpDir);
    common_dir = new char[strlen(cDir) + 1];
    strcpy(common_dir,cDir);
    log_file = new char[strlen(logfile) + 1];
    strcpy(log_file,logfile);
    logF = fopen(log_file,"a");
    idirPtr = opendir(inp_dir);

}
// Connect to the fifo , if it doesnt exist creates it
int Writer::connect(void) {
    char fifo_file[256] ;
    sprintf(fifo_file,"%s/%d_to_%d.fifo",common_dir,from,to);
    mkfifo(fifo_file, 0666);
    pipeD = open(fifo_file, O_WRONLY);
    return 0 ;

}
int Writer::sendFile(char *dir,char *name) {
    // We create  copy of dir because strtok modifies it
    char *dir2 = new char[strlen(dir) + 1];
    strcpy(dir2,dir);
    char *strippedpath = strtok(dir2,"/");
    strippedpath = strtok(NULL,"");

    // We differentiate between the relative path and what we want to send
    // When we send the name of the file we remove the input dir
    char inpath[512] ;
    sprintf(inpath,"%s/%s",dir,name);
    struct stat st;
    stat(inpath, &st);
    int fSize = st.st_size;
    int fd = open(inpath,O_RDONLY);
    char fName[512] ;
    if (strippedpath == NULL) {
        sprintf(fName,"./%s",name);
    } else {
        sprintf(fName,"%s/%s",strippedpath,name);
    }
    short tLen = (short)strlen(fName);
    //fprintf(logF,"File Name : %s , %hd\n",fName ,(short)strlen(fName));
    fflush(stdout);
    write(pipeD,&tLen,sizeof(short));
    write(pipeD,fName,strlen(fName));
    write(pipeD,&fSize,sizeof(int));
    int count = fSize ;
    while(count > 0) {
        char *contents = new char[buff+1];
        memset(contents,'\0',buff+1);
        read(fd,contents,buff);
        write(pipeD,contents,buff);
        count -= buff ;
        delete contents ;
    }
    flock(fileno(logF),LOCK_EX);
    fprintf(logF, "bytes_sent %d\n",fSize + (int)tLen);
    flock(fileno(logF),LOCK_UN);
    flock(fileno(logF),LOCK_EX);
    fprintf(logF, "send_file\n");
    flock(fileno(logF),LOCK_UN);

    close(fd);
    delete dir2;
    //fprintf(logF, "Dir %s . Name %s \n",strippedpath,name );
    return 0 ;
}
// Recursively goes through directories in inp_dir 
int Writer::sendFilesInDir(char *dirpath) {

    DIR * dir = opendir(dirpath);
    if (dir == NULL) {
        fprintf(stderr, "NULL dir\n");
    }
    //fprintf(logF,"Dirpath sendFilesInDir : %s \n",dirpath);
    struct dirent *ind ;
    while((ind = readdir(dir)) != NULL) {
        //fprintf(stdout,"Dirpath : %s\n",ind->d_name );
        fflush(stdout);
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
            sendFilesInDir(totalpath);
            continue ;
        }
        //fprintf(logF,"Path : %s\n",totalpath );
        //fflush(logF);
        // We check if it
        sendFile(dirpath,ind->d_name);
    }
    closedir(dir);
    return 0 ;
}
int Writer::sendFiles(void) {
    sendFilesInDir(inp_dir);
    short end = 0 ;
    write(pipeD,&end,2);

    close(pipeD);
    return 0 ;
}
Writer::~Writer() {
    flock(fileno(logF),LOCK_EX);
    fprintf(logF, "Exiting Writer \n");
    flock(fileno(logF),LOCK_UN);
    fclose(logF);
    closedir(idirPtr);
    delete common_dir ;
    delete inp_dir ;
    delete log_file ;
}
