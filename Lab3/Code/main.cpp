#include <cstdio>
#include <omp.h>
#include <string>

#include "IO.h"
#include "timer.h"

int i, j, k, size;
double **Au;
double *X;
double temp, error, Xnorm;
int *index;
void rref(double **Au, double *X, int size) {

    /*Calculate the solution by serial code*/
    index = (int *) malloc(size * sizeof(int));
    for (i = 0; i < size; ++i)
        index[i] = i;

    if (size == 1)
        X[0] = Au[0][1] / Au[0][0];
    else {
        /*Gaussian elimination*/
        for (k = 0; k < size - 1; ++k) {
            /*Pivoting*/
            temp = 0;
            for (i = k, j = 0; i < size; ++i)
                if (temp < Au[index[i]][k] * Au[index[i]][k]) {
                    temp = Au[index[i]][k] * Au[index[i]][k];
                    j = i;
                }
            if (j != k) /*swap*/ {
                i = index[j];
                index[j] = index[k];
                index[k] = i;
            }
            /*calculating*/
            for (i = k + 1; i < size; ++i) {
                temp = Au[index[i]][k] / Au[index[k]][k];
                for (j = k; j < size + 1; ++j)
                    Au[index[i]][j] -= Au[index[k]][j] * temp;
            }
        }
        /*Jordan elimination*/
        for (k = size - 1; k > 0; --k) {
            for (i = k - 1; i >= 0; --i) {
                temp = Au[index[i]][k] / Au[index[k]][k];
                Au[index[i]][k] -= temp * Au[index[k]][k];
                Au[index[i]][size] -= temp * Au[index[k]][size];
            }
        }

        /*solution*/
        for (k = 0; k < size; ++k) {
            X[k] = Au[index[k]][size] / Au[index[k]][k];
            printf("Print matrix result\n %lf", X[k]);
        }
    }
}

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