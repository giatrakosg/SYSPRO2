#include "Writer.hpp"

int main(int argc, char *argv[]) {
    if (argc < 5) {
        fprintf(stderr, "Incorrect number of parameters\n");
        return -1 ;
    }
    //sleep(10);
    int buff = atoi(argv[1]);
    int fromID = atoi(argv[2]);
    int toID = atoi(argv[3]);
    Writer writer(buff,fromID,toID,argv[4],argv[5],argv[6]);
    writer.connect();
    int err = writer.sendFiles();
    if (err < 0) {
        kill(getppid(),SIGUSR1);
    }
    return 0;
}
