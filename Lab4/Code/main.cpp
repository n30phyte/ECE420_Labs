#include "mpi.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
  MPI::Init(argc, argv);

  int world_size = MPI::COMM_WORLD.Get_size();
  int world_rank = MPI::COMM_WORLD.Get_rank();

  printf("Hello from process %d out of %d\n", world_rank, world_size);
  MPI::Finalize();
  return 0;
}
