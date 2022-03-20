/*
    Test the result stored in "data_output" against a serial implementation.

    -----
    Compiling:
    Include "Lab4_IO.c" to compile. Set the macro "LAB4_EXTEND" defined in the
   "Lab4_IO.c" file to include the extended functions $ gcc serialtester.c
   Lab4_IO.c -o serialtester -lm

    -----
    Return values:
    0      result is correct
    1      result is wrong
    2      problem size does not match
    253    no "data_output" file
    254    no "data_input_meta" or "data_input_link" file
*/

#include "IO.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define EPSILON 0.00001
#define DAMPING_FACTOR 0.85

#define THRESHOLD 0.0001

struct node {
  int *inlinks;
  int num_in_links;
  int num_out_links;
};

int node_init(struct node **nodehead, int start, int end) {
  FILE *ip;
  int i, nodecount;
  int src, dst;
  int nodeID, num_in, num_out;
  int *index; // keep track of the outlink list storing position
  int num_nodes = end - start;

  // deal with the meta data and allocate space
  (*nodehead) = malloc(num_nodes * sizeof(struct node));
  if ((ip = fopen("data_input_meta", "r")) == NULL) {
    printf("Error opening the data_input_meta file.\n");
    return -1;
  }
  fscanf(ip, "%d\n", &nodecount);
  for (i = 0; i < start; ++i) { // allocate to the right place
    fscanf(ip, "%d\t%d\t%d\n", &nodeID, &num_in, &num_out);
  }
  for (i = start; i < end && i < nodecount; ++i) { // handlling the dumping case
    fscanf(ip, "%d\t%d\t%d\n", &nodeID, &num_in, &num_out);
    if (nodeID != i) {
      printf("Error loading meta data, node id inconsistent!\n");
      return -2;
    }
    (*nodehead)[i - start].num_in_links = num_in;
    (*nodehead)[i - start].num_out_links = num_out;
    (*nodehead)[i - start].inlinks = malloc(num_in * sizeof(int));
  }
  fclose(ip);
  if (i < end) { // initial the dampling nodes as well
    for (; i < end; ++i) {
      (*nodehead)[i - start].num_in_links = 0;
      (*nodehead)[i - start].num_out_links = nodecount;
      (*nodehead)[i - start].inlinks = malloc(sizeof(int));
    }
  }
  // Load the link informations
  if ((ip = fopen("data_input_link", "r")) == NULL) {
    printf("Error opening the data_input_link file.\n");
    return -3;
  }
  index = malloc(num_nodes * sizeof(int));
  for (i = 0; i < num_nodes; ++i) { index[i] = 0; }
  while (!feof(ip)) {
    fscanf(ip, "%d\t%d\n", &src, &dst);
    if (dst >= start && dst < end) {
      (*nodehead)[dst - start].inlinks[index[dst - start]++] = src;
    }
  }
  free(index);
  fclose(ip);
  return 0;
}

int node_destroy(struct node *nodehead, int num_nodes) {
  int i;
  for (i = 0; i < num_nodes; ++i) { free(nodehead[i].inlinks); }
  free(nodehead);
  return 0;
}

double rel_error(double *r, double *t, int size) {
  int i;
  double norm_diff = 0, norm_vec = 0;
  for (i = 0; i < size; ++i) {
    norm_diff += (r[i] - t[i]) * (r[i] - t[i]);
    norm_vec += t[i] * t[i];
  }
  return sqrt(norm_diff / norm_vec);
}

int vec_cp(double *r, double *t, int size) {
  int i;
  for (i = 0; i < size; ++i) t[i] = r[i];
  return 0;
}

int main(int argc, char *argv[]) {
  struct node *nodehead;
  int nodecount;
  double *r, *r_pre, *contribution;
  int i, j;
  double damp_const;
  int iterationcount = 0;
  int collected_nodecount;
  double *collected_r;
  double cst_addapted_threshold;
  double error;
  FILE *fp, *ip;

  // Load the data and simple verification
  if ((fp = fopen("data_output", "r")) == NULL) {
    printf("Error loading the data_output.\n");
    return 253;
  }
  fscanf(fp, "%d\n%lf\n", &collected_nodecount, &error);
  if ((ip = fopen("data_input_meta", "r")) == NULL) {
    printf("Error opening the data_input_meta file.\n");
    return 254;
  }
  fscanf(ip, "%d\n", &nodecount);
  if (nodecount != collected_nodecount) {
    printf("Problem size does not match!\n");
    return 2;
  }
  fclose(ip);
  collected_r = malloc(collected_nodecount * sizeof(double));
  for (i = 0; i < collected_nodecount; ++i)
    fscanf(fp, "%lf\n", &collected_r[i]);
  fclose(fp);

  // Adjust the threshold according to the problem size
  cst_addapted_threshold = THRESHOLD;

  if (node_init(&nodehead, 0, nodecount)) return 254;
  // initialize variables
  r = malloc(nodecount * sizeof(double));
  r_pre = malloc(nodecount * sizeof(double));
  for (i = 0; i < nodecount; ++i) r[i] = 1.0 / nodecount;
  contribution = malloc(nodecount * sizeof(double));
  for (i = 0; i < nodecount; ++i)
    contribution[i] = r[i] / nodehead[i].num_out_links * DAMPING_FACTOR;
  damp_const = (1.0 - DAMPING_FACTOR) / nodecount;
  // CORE CALCULATION
  // GET_TIME(start);
  do {
    ++iterationcount;
    vec_cp(r, r_pre, nodecount);
    // update the value
    for (i = 0; i < nodecount; ++i) {
      r[i] = 0;
      for (j = 0; j < nodehead[i].num_in_links; ++j)
        r[i] += contribution[nodehead[i].inlinks[j]];
      r[i] += damp_const;
    }
    // update and broadcast the contribution
    for (i = 0; i < nodecount; ++i) {
      contribution[i] = r[i] / nodehead[i].num_out_links * DAMPING_FACTOR;
    }
  } while (rel_error(r, r_pre, nodecount) >= EPSILON);
  // GET_TIME(end);
  // printf("Program converged at %d th iteration.\nElapsed time %f.\n",
  // iterationcount, end-start);

  // post processing
  node_destroy(nodehead, nodecount);
  free(contribution);

  // Compare the result
  error = rel_error(collected_r, r, nodecount);
  printf("The relative error against the reference result is %e .\n", error);
  free(r);
  free(r_pre);
  free(collected_r);
  if (error < cst_addapted_threshold) {
    printf("Congratulations! Your result is correct!\n");
    return 0;
  } else {
    printf("Sorry. Your result is wrong.\n");
    return 1;
  }
}
