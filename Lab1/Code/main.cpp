#include "IO.h"
#include "timer.h"

#include <cmath>
#include <iostream>
#include <pthread.h>
#include <unistd.h>

struct thread_inputs_t {
    int **A;
    int **B;
    int **C;
    int n;
    int k;
    int p;
};

void *mat_mul(void *inputs) {
    auto *input_struct = (thread_inputs_t *) inputs;

    int **A = input_struct->A;
    int **B = input_struct->B;
    int **C = input_struct->C;
    int n = input_struct->n;
    int k = input_struct->k;
    int p = input_struct->p;

    auto x = (int) floor(k / sqrt(p));
    auto y = k % (int) sqrt(p);

    auto i_start = (n / (int) sqrt(p)) * x;
    auto i_end = ((n / (int) sqrt(p)) * (x + 1)) - 1;

    auto j_start = (n / (int) sqrt(p)) * y;
    auto j_end = ((n / (int) sqrt(p)) * (y + 1)) - 1;

    for (auto i = i_start; i <= i_end; i++) {
        for (auto j = j_start; j <= j_end; j++) {
            int sum = 0;
            for (auto r = 0; r < n; r++) {
                int a = A[i][r];
                int b = B[r][j];
                sum += a * b;
            }
            C[i][j] = sum;
        }
    }

    return nullptr;
}

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Please input the number of threads to use.\n");
        return 1;
    }

    auto num_threads = atoi(argv[1]);
    printf("Using %d threads\n", num_threads);

    int **A;
    int **B;
    int n;

    LoadInput(&A, &B, &n);

    int **C = (int **) malloc(n * sizeof(int *));

    for (auto row = 0; row < n; row++) {
        C[row] = (int *) malloc(n * sizeof(int *));
    }

    auto thread_handles = (pthread_t *) malloc(num_threads * sizeof(pthread_t));

    double start_time;
    GET_TIME(start_time)

    for (auto i = 0; i < num_threads; i++) {
        pthread_t handle;
        auto *input_struct = (thread_inputs_t *) (malloc(sizeof(thread_inputs_t)));
        *input_struct = {A, B, C, n, i, num_threads};
        pthread_create(&handle, nullptr, mat_mul, (void *) input_struct);
        thread_handles[i] = handle;
    }

    for (auto i = 0; i < num_threads; i++) {
        pthread_join(thread_handles[i], nullptr);
    }
    double end_time;
    GET_TIME(end_time)

    SaveOutput(C, &n, end_time - start_time);

    return 0;
}