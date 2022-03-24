
#include "IO.h"
#include "timer.h"
#include "utils.h"

#include <algorithm>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <mpi.h>

#define EPSILON 0.00001
#define DAMPING_FACTOR 0.85

#define THRESHOLD 0.0001

void pagerank() {

  int nodecount;
  std::fstream data_input("data_input_meta");
  data_input >> nodecount;
  data_input.close();

  node *nodehead;
  node_init(&nodehead, 0, nodecount);
  // initialize variables
  double *r = new double[nodecount];
  double *r_pre = new double[nodecount];
  double *contribution = new double[nodecount];

  for (int i = 0; i < nodecount; ++i) {
    r[i] = 1.0 / nodecount;
    contribution[i] = r[i] / nodehead[i].num_out_links * DAMPING_FACTOR;
  }

  double damping_constant = (1.0 - DAMPING_FACTOR) / nodecount;

  int iteration_counter = 0;
  int my_rank = MPI::COMM_WORLD.Get_rank();
  int comm_sz = MPI::COMM_WORLD.Get_size();

  int local_nodecount = (nodecount / comm_sz);

  double *local_r = new double[local_nodecount];
  double *local_contribution = new double[local_nodecount];

  // CORE CALCULATION
  double start, end;
  GET_TIME(start);
  do {
    ++iteration_counter;
    // update the value
    std::copy_n(r, nodecount, r_pre);

    for (int i = 0; i < local_nodecount; ++i) {
      local_r[i] = 0;
      for (int j = 0; j < nodehead[i + local_nodecount * my_rank].num_in_links;
           ++j) {
        local_r[i] +=
            contribution[nodehead[i + local_nodecount * my_rank].inlinks[j]];
      }
      local_r[i] += damping_constant;
    }
    // update and broadcast the contribution
    for (int i = 0; i < local_nodecount; ++i) {
      local_contribution[i] = local_r[i] /
          nodehead[i + local_nodecount * my_rank].num_out_links *
          DAMPING_FACTOR;
    }

    MPI::COMM_WORLD.Allgather(local_contribution, local_nodecount, MPI_DOUBLE,
                              contribution, local_nodecount, MPI_DOUBLE);

    MPI::COMM_WORLD.Allgather(local_r, local_nodecount, MPI_DOUBLE, r,
                              local_nodecount, MPI_DOUBLE);

  } while (rel_error(r, r_pre, nodecount) >= EPSILON);
  GET_TIME(end);

#ifdef DEBUG
  std::cout << "Program converged at " << iteration_counter << "th iteration."
            << std::endl
            << "Elapsed time: " << end - start << std::endl;
#endif
  // post processing
  node_destroy(nodehead, nodecount);

  if (my_rank == 0) { SaveOutput(r, nodecount, end - start); }

  delete[] r;
  delete[] local_r;
  delete[] r_pre;
  delete[] contribution;
}

int main(int argc, char *argv[]) {
  MPI::Init(argc, argv);

  pagerank();

  MPI::Finalize();

  return 0;
}
