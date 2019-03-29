
#include "Client.hpp"
#include <signal.h>
bool runFlag = true ;

void sigstop_handler(int signum) {
    signal(SIGINT,sigstop_handler);
    signal(SIGQUIT,sigstop_handler);
    std::cout << "Bye" <<std::endl ;
    runFlag = false ;
}



int main(int argc, char *argv[]) {
    signal(SIGQUIT,sigstop_handler);
    signal(SIGINT,sigstop_handler);

    Client *cli = new Client;
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
    while (runFlag) {
        //sleep(20);
        cli->listen();
    }

    delete cli ;
    return 0;
}
