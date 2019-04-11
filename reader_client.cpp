#include "Reader.hpp"
#include <iostream>

void sigalrm_handler(int signum) {
    printf("Received alarm %d\n",signum );
    signal(SIGALRM,sigalrm_handler);
    kill(getppid(),SIGUSR1);
    exit(-1);
}

int main(int argc, char *argv[]) {
    //printf("Created reader proc \n");
    signal(SIGALRM,sigalrm_handler);
    //alarm(2);
    if (argc < 5) {
        fprintf(stderr, "Not enough arguments \n");
        return -1 ;
    }

    int buff = atoi(argv[1]);
    int fromID = atoi(argv[2]);
    int toID = atoi(argv[3]);
    class Reader reader(buff,fromID,toID,argv[4],argv[5],argv[6]) ;
    reader.createPipe();
    reader.readFromPipe();

    //printf("Exiting READER\n");
    return 0;
}
