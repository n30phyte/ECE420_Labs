#include <cstdio>
#include <omp.h>
#include <string>

#include "IO.h"
#include "timer.h"

void rref(double **G, double *X, int n) {}

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Please input the number of threads to use.\n");
        return 1;
    }

    auto num_threads = std::stoi(argv[1]);
    printf("Using %d threads\n", num_threads);

    omp_set_num_threads(num_threads);

    double **G;
    int size;

    LoadInput(&G, &size);

    double *X = CreateVec(size);

    double start, finish;

    GET_TIME(start);
    {
        rref(G, X, size);
    }
    GET_TIME(finish);

    SaveOutput(X, size, finish - start);
}