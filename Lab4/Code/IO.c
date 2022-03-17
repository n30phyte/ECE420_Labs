#include "IO.h"
#include <stdio.h>

/**
 * Save the data to file for Lab 4
 * @param R pointer to the result array
 * @param nodecount number of nodes
 * @param Time measured calculation time
 * @return 0 for success, 1 otherwise
 */
int SaveOutput(double *R, int nodecount, double Time) {
  FILE *op;
  int i;

  if ((op = fopen("data_output", "w")) == NULL) {
    printf("Error opening the input file.\n");
    return 1;
  }
  fprintf(op, "%d\n%f\n", nodecount, Time);
  for (i = 0; i < nodecount; ++i) fprintf(op, "%e\n", R[i]);
  fclose(op);
  return 0;
}
