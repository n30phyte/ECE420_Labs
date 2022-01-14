#include "IO.h"

#include <pthread.h>

#include <iostream>

int main(int argc, char *argv[]) {
    std::cout << "Num arguments: " << argc << std::endl;
    if (argc != 2) {
        std::cout << "Need to input number of threads." << std::endl;
        return 1;
    }
    std::cout << "Threads used: " << argv[argc - 1] << std::endl;

    int **A;
    int **B;
    int n;

    LoadInput(&A, &B, &n);

    return 0;
}