#include <cstdio>
#include <omp.h>
#include <string>

#include "IO.h"
#include "timer.h"

void rref(double **G, double *X, int *index, int size) {
    if (size == 1) {
        X[0] = G[0][1] / G[0][0];
    } else {
        // Gaussian Elimination
        for (int k = 0; k < size - 1; ++k) {
            // Pivot
            // Pivot Pt 1: Find Max
            double max_val = 0;
            int j = 0;

            // Serial
            // for (int i = k; i < size; ++i) {
            //     if (max_val < G[index[i]][k] * G[index[i]][k]) {
            //         max_val = G[index[i]][k] * G[index[i]][k];
            //         j = i;
            //     }
            // }

#pragma omp parallel shared(max_val, j)
            {
                int max_local = 0;
                int j_local = j;
#pragma omp for
                for (int i = k; i < size; ++i) {
                    if (max_local < G[index[i]][k] * G[index[i]][k]) {
                        max_local = G[index[i]][k] * G[index[i]][k];
                        j_local = i;
                    }
                }
                if (max_val < max_local) {
#pragma omp critical
                    if (max_val < max_local) {
                        max_val = max_local;
                        j = j_local;
                    }
                }
            }

            // Pivot Pt 2: Swap
            if (j != k) {
                int temp_row = index[j];
                index[j] = index[k];
                index[k] = temp_row;
            }

            // Elimination
#pragma omp parallel for
            for (int i = k + 1; i < size; ++i) {
                double temp = G[index[i]][k] / G[index[k]][k];
                for (int j = k; j < size + 1; ++j)
                    G[index[i]][j] -= G[index[k]][j] * temp;
            }
        }

        double **D = G;

        // Jordan Elimination
        for (int k = size - 1; k > 0; --k) {
#pragma omp parallel for
            for (int i = k - 1; i >= 0; --i) {
                double ratio = D[index[i]][k] / D[index[k]][k];
                D[index[i]][k] -= ratio * D[index[k]][k];
                D[index[i]][size] -= ratio * D[index[k]][size];
            }
        }

// Save solution
#pragma omp parallel for
        for (int i = 0; i < size; i++) {
            X[i] = D[index[i]][size] / D[index[i]][i];
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
    int *index = new int[size];
    for (int i = 0; i < size; ++i)
        index[i] = i;

    double start, finish;

    GET_TIME(start);
    {
        rref(G, X, index, size);
    }
    GET_TIME(finish);

    SaveOutput(X, size, finish - start);
}
