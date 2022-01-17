#include "IO.h"

#include <iostream>
#include <pthread.h>
#include <vector>

struct thread_inputs_t {
    int **A;
    int **B;
    int **C;
    int n;
    unsigned int thread_num;
};

void *mat_mul(void *inputs) {
    auto *input_struct = (thread_inputs_t *) inputs;

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

    std::vector<pthread_t> thread_handles(num_threads);

    for (auto i = 0; i < n; i++) {
        pthread_t handle;
        auto *input_struct = (thread_inputs_t *) (malloc(sizeof(thread_inputs_t)));
        *input_struct = {A, B, C, n};
        input_struct->thread_num = i;
        pthread_create(&handle, nullptr, mat_mul, (void *) input_struct);
    }

    return 0;
}