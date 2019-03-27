
#include "Client.hpp"

int main(int argc, char *argv[]) {
    class Client cli ;
    cli.getArgs(argc,argv);
    cli.printArgs();
    return 0;
}
