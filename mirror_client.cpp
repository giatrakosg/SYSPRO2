
#include "Client.hpp"
#include <signal.h>
#define SLEEP_PERIOD 3

bool runFlag ;

void sigstop_handler(int signum) {
    signal(SIGINT,sigstop_handler);
    signal(SIGQUIT,sigstop_handler);
    runFlag = false ;
}
void sigusr_handler(int signum){
    signal(SIGUSR1,sigusr_handler);
}


int main(int argc, char *argv[]) {
    Client *cli = new Client ;
    runFlag = true ;

    signal(SIGQUIT,sigstop_handler);
    signal(SIGINT,sigstop_handler);
    signal(SIGUSR1,sigusr_handler);
    cli->getArgs(argc,argv);
    //cli->printArgs();
    int err = cli->parseArgs();
    if (err < 0) {
        delete cli ;
        return -1;
    }
    err = cli->writeID();
    if (err < 0) {
        delete cli ;
        return -1 ;
    }
    while (runFlag == true) {
        sleep(SLEEP_PERIOD);
        cli->detectNewID();
    }

    delete cli ;
    return 0;
}
