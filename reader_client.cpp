#include "Reader.hpp"
#include <iostream>

int main(int argc, char *argv[]) {
    if (argc < 5) {
        fprintf(stderr, "Not enough arguments \n");
        return -1 ;
    }
    for (int i = 0; i < argc; i++) {
        std::cout << argv[i] << std::endl ;
    }
    int fromID = atoi(argv[1]);
    int toID = atoi(argv[2]);
    class Reader reader(fromID,toID,argv[3],argv[4]) ;
    reader.createPipe();

    return 0;
}
