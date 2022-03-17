#include "mpi.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
  int myrank, npes;
  printf("Hello from process %d out of %d\n", myrank, npes);

  MPI::Init(argc, argv);

  npes = MPI::COMM_WORLD.Get_size();
  myrank = MPI::COMM_WORLD.Get_rank();

  printf("Hello from process %d out of %d\n", myrank, npes);
  MPI_Finalize();
  printf("Hello from process %d out of %d\n", myrank, npes);
  return 0;
}
