
#include "timer.h"
#include "utils.h"

#include <cstdio>
#include <fstream>
#include <iostream>
#include <mpi.h>

#define EPSILON 0.00001
#define DAMPING_FACTOR 0.85

#define THRESHOLD 0.0001

void serialmain() {
  FILE *ip;
  int nodecount;
  if ((ip = fopen("data_input_meta", "r")) == NULL) {
    printf("Error opening the data_input_meta file.\n");
  }
  fscanf(ip, "%d\n", &nodecount);
  fclose(ip);

  // Adjust the threshold according to the problem size
  double cst_adapted_threshold = THRESHOLD;

  node *nodehead;
  node_init(&nodehead, 0, nodecount);
  // initialize variables
  double *r = new double[nodecount];
  double *r_pre = new double[nodecount];
  for (int i = 0; i < nodecount; ++i) { r[i] = 1.0 / nodecount; }

  double *contribution = new double[nodecount];

  for (int i = 0; i < nodecount; ++i) {
    contribution[i] = r[i] / nodehead[i].num_out_links * DAMPING_FACTOR;
  }
  double damp_const = (1.0 - DAMPING_FACTOR) / nodecount;

  double start, end;
  // CORE CALCULATION
  int iterationcount = 0;
  GET_TIME(start);
  do {
    ++iterationcount;
    vec_cp(r, r_pre, nodecount);
    // update the value
    for (int i = 0; i < nodecount; ++i) {
      r[i] = 0;
      for (int j = 0; j < nodehead[i].num_in_links; ++j) {
        r[i] += contribution[nodehead[i].inlinks[j]];
      }
      r[i] += damp_const;
    }
    // update and broadcast the contribution
    for (int i = 0; i < nodecount; ++i) {
      contribution[i] = r[i] / nodehead[i].num_out_links * DAMPING_FACTOR;
    }
  } while (rel_error(r, r_pre, nodecount) >= EPSILON);
  GET_TIME(end);
  printf("Program converged at %d th iteration.\nElapsed time %f.\n",
         iterationcount, end - start);

  // post processing
  node_destroy(nodehead, nodecount);

  delete[] r;
  delete[] r_pre;
  delete[] contribution;
}

int main(int argc, char *argv[]) {
  MPI::Init(argc, argv);

  int world_size = MPI::COMM_WORLD.Get_size();
  int world_rank = MPI::COMM_WORLD.Get_rank();

  printf("Hello from process %d out of %d\n", world_rank, world_size);

  serialmain();

  MPI::Finalize();

  return 0;
}
