
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

  node *nodehead, *local_nodehead;
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
  int my_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  int comm_sz;
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

  GET_TIME(start);
  do {
    ++iterationcount;
    vec_cp(r, r_pre, nodecount);
    // update the value
    int local_nodecount = (nodecount / comm_sz);
    int wtf[4] = {0, 2500, 5000, 7500};
    int wtf2[4] = {2500, 5000, 7500, 10000};

    for (int i = wtf[my_rank]; i < wtf2[my_rank]; ++i) {
      r[i] = 0;
      for (int j = 0; j < nodehead[i].num_in_links; ++j) {
        r[i] += contribution[nodehead[i].inlinks[j]];
      }
      r[i] += damp_const;
    }
    // update and broadcast the contribution
    for (int i = wtf[my_rank]; i < wtf2[my_rank]; ++i) {
      contribution[i] = r[i] / nodehead[i].num_out_links * DAMPING_FACTOR;
      // MPI_Gatherv
    }

    // MPI_Reduce();
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

void mpitest() {
  const int MAX_STRING = 100;

  char greeting[MAX_STRING]; /* String storing message */
  int comm_sz;               /* Number of processes    */
  int my_rank;
  /* Get the number of processes */
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

  /* Get my rank among all the processes */
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  int in_nodes[2];
  int *out_nodes_fake;

  int out_nodes_true[8] = {0, 1, 2, 3, 4, 5, 6, 7};
  if (my_rank == 0) {
    MPI_Scatter(out_nodes_true, 2, MPI_INT, in_nodes, 8, MPI_INT, 0,
                MPI_COMM_WORLD);
  } else {
    MPI_Scatter(out_nodes_fake, 2, MPI_INT, in_nodes, 8, MPI_INT, 0,
                MPI_COMM_WORLD);
  }
  printf("Greetings from process %d, my element is %d !\n", my_rank,
         in_nodes[0]);

  // if (my_rank != 0) {
  //   /* Create message */
  //   sprintf(greeting, "Greetings from process %d of %d!", my_rank, comm_sz);
  //   /* Send message to process 0 */
  //   MPI_Send(greeting, strlen(greeting) + 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
  // } else {
  //   /* Print my message */
  //   printf("Greetings from master process %d of %d!\n", my_rank, comm_sz);
  //   int q;
  //   for (q = 1; q < comm_sz; q++) {
  //     /* Receive message from process q */
  //     MPI_Recv(greeting, MAX_STRING, MPI_CHAR, q, 0, MPI_COMM_WORLD,
  //              MPI_STATUS_IGNORE);
  //     /* Print message from process q */
  //     printf("%s\n", greeting);
  //   }
}

int main(int argc, char *argv[]) {
  MPI::Init(argc, argv);

  int world_size = MPI::COMM_WORLD.Get_size();
  int world_rank = MPI::COMM_WORLD.Get_rank();

  printf("Hello from process %d out of %d\n", world_rank, world_size);

  serialmain();
  // mpitest();

  MPI::Finalize();

  return 0;
}
