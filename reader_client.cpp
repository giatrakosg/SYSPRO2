#include "Reader.hpp"
#include <iostream>

int main(int argc, char *argv[]) {
    printf("Created reader proc \n");
    if (argc < 5) {
        fprintf(stderr, "Not enough arguments \n");
        return -1 ;
    }
    for (int i = 0; i < argc; i++) {
        std::cout << argv[i] << std::endl ;
    }

    int buff = atoi(argv[1]);
    int fromID = atoi(argv[2]);
    int toID = atoi(argv[3]);
    class Reader reader(buff,fromID,toID,argv[4],argv[5]) ;
    reader.createPipe();
    reader.readFromPipe();

    printf("Exiting READER\n");
    return 0;
}
