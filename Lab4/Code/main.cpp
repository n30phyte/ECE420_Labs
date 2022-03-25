
#include "IO.h"
#include "timer.h"
#include "utils.h"

#include <mpi.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>

const double EPSILON = 0.00001;
const double DAMPING_FACTOR = 0.85;

const double THRESHOLD = 0.0001;

std::vector<int> generate_nodecounts(int total_nodecount, int workers) {
  std::vector<int> node_counts;

  for (auto i = 0; i < workers; i++) {
    node_counts.push_back(total_nodecount / workers);
  }

  if ((total_nodecount % workers) > 0) {
    for (int i = 0; i < (total_nodecount % workers); i++) { node_counts[i]++; }
  }

  return node_counts;
}

std::vector<int> exclusive_scan(const std::vector<int> &node_counts) {
  std::vector<int> displacements;

  displacements.resize(node_counts.size());

  std::partial_sum(node_counts.begin(), node_counts.end(),
                   displacements.begin(), std::plus<int>());

  displacements.insert(displacements.begin(), 1, 0);
  displacements.pop_back();

  return displacements;
}

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
  int worker_rank = MPI::COMM_WORLD.Get_rank();
  int total_workers = MPI::COMM_WORLD.Get_size();

  std::vector<int> node_counts = generate_nodecounts(nodecount, total_workers);
  std::vector<int> displacements = exclusive_scan(node_counts);

  int local_nodecount = node_counts[worker_rank];

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

      for (int j = 0; j < nodehead[i + displacements[worker_rank]].num_in_links;
           ++j) {
        local_r[i] +=
            contribution[nodehead[i + displacements[worker_rank]].inlinks[j]];
      }
      local_r[i] += damping_constant;
    }
    // update and broadcast the contribution
    for (int i = 0; i < local_nodecount; ++i) {
      local_contribution[i] = local_r[i] /
          nodehead[i + displacements[worker_rank]].num_out_links *
          DAMPING_FACTOR;
    }

    MPI::COMM_WORLD.Allgatherv(local_contribution, local_nodecount, MPI_DOUBLE,
                               contribution, node_counts.data(),
                               displacements.data(), MPI_DOUBLE);

    MPI::COMM_WORLD.Allgatherv(local_r, local_nodecount, MPI_DOUBLE, r,
                               node_counts.data(), displacements.data(),
                               MPI_DOUBLE);

  } while (rel_error(r, r_pre, nodecount) >= EPSILON);
  GET_TIME(end);

#ifdef DEBUG
  std::cout << "Program converged at " << iteration_counter << "th iteration."
            << std::endl
            << "Elapsed time: " << end - start << std::endl;
#endif
  // post processing
  node_destroy(nodehead, nodecount);

  if (worker_rank == 0) { SaveOutput(r, nodecount, end - start); }

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
