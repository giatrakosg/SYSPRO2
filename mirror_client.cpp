
#include "Client.hpp"

int main(int argc, char *argv[]) {
    Client *cli = new Client;
    cli->getArgs(argc,argv);
    cli->printArgs();
    int err = cli->parseArgs();
    if (err < 0) {
        return -1;
    }
    delete cli ;
    return 0;
}
