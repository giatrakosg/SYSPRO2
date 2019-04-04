#include "Writer.hpp"

int main(int argc, char *argv[]) {
    int buff = atoi(argv[1]);
    int fromID = atoi(argv[2]);
    int toID = atoi(argv[3]);
    Writer writer(buff,fromID,toID,argv[4],argv[5]);
    writer.connect();
    writer.sendFile("./hello");
    return 0;
}
