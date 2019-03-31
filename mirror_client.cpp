
#include "Client.hpp"
#include <signal.h>
bool runFlag ;
Client *cli ;

void sigstop_handler(int signum) {
    signal(SIGINT,sigstop_handler);
    signal(SIGQUIT,sigstop_handler);
    std::cout << "Bye" <<std::endl ;
    runFlag = false ;
    delete cli ;
    exit(0);
}



int main(int argc, char *argv[]) {
    runFlag = true ;

    signal(SIGQUIT,sigstop_handler);
    signal(SIGINT,sigstop_handler);

    cli = new Client;
    cli->getArgs(argc,argv);
    cli->printArgs();
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
        //sleep(20);
        cli->detectNewID();
    }

    delete cli ;
    return 0;
}
