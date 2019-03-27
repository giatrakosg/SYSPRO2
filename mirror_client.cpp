
#include "Client.hpp"

int main(int argc, char *argv[]) {
    class Client cli ;
    cli.getArgs(argc,argv);
    cli.printArgs();
    int err = cli.parseArgs();
    if (err < 0) {
        exit(-1);
    }
    return 0;
}
