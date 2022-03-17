#include "mpi.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
    int npes, myrank;
    printf("Hello from process %d out of %d\n", myrank, npes);

    MPI_Init(&argc, &argv);

    /* Get the number of processes */
    MPI_Comm_size(MPI_COMM_WORLD, &npes);
    /* Get my rank among all the processes */
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    printf("Hello from process %d out of %d\n", myrank, npes);
    MPI_Finalize();
    printf("Hello from process %d out of %d\n", myrank, npes);
    return 0;
}
