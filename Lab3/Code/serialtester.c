/*
Test the result stored in the "data_output" by a serial version of calculation

-----
Compiling:
	"Lab3IO.c" should be included and "-lm" tag is needed, like
	> gcc serialtester.c Lab3IO.c -o serialtester -lm
*/
#include "IO.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define TOL 0.0005

int main(int argc, char *argv[]) {
    int i, j, k, size;
    double **Au;
    double *X;
    double temp, error, Xnorm;
    int *index;
    FILE *fp;

    /*Load the datasize and verify it*/
    LoadInput(&Au, &size);
    if ((fp = fopen("data_output", "r")) == NULL) {
        printf("Fail to open the result data file!\n");
        return 2;
    }
    fscanf(fp, "%d\n\n", &i);
    if (i != size) {
        printf("The problem size of the input file and result file does not match!\n");
        return -1;
    }
    /*Calculate the solution by serial code*/
    X = CreateVec(size);
    index = malloc(size * sizeof(int));
    for (i = 0; i < size; ++i)
        index[i] = i;

    if (size == 1)
        X[0] = Au[0][1] / Au[0][0];
    else {
        /*Gaussian elimination*/
        for (k = 0; k < size - 1; ++k) {
            /*Pivoting*/
            temp = 0;
            for (i = k, j = 0; i < size; ++i)
                if (temp < Au[index[i]][k] * Au[index[i]][k]) {
                    temp = Au[index[i]][k] * Au[index[i]][k];
                    j = i;
                }
            if (j != k) /*swap*/ {
                i = index[j];
                index[j] = index[k];
                index[k] = i;
            }
            /*calculating*/
            for (i = k + 1; i < size; ++i) {
                temp = Au[index[i]][k] / Au[index[k]][k];
                for (j = k; j < size + 1; ++j)
                    Au[index[i]][j] -= Au[index[k]][j] * temp;
            }
        }
        /*Jordan elimination*/
        for (k = size - 1; k > 0; --k) {
            for (i = k - 1; i >= 0; --i) {
                temp = Au[index[i]][k] / Au[index[k]][k];
                Au[index[i]][k] -= temp * Au[index[k]][k];
                Au[index[i]][size] -= temp * Au[index[k]][size];
            }
        }
        /*solution*/
        for (k = 0; k < size; ++k)
            X[k] = Au[index[k]][size] / Au[index[k]][k];
    }

    /*compare the solution*/
    error = 0;
    Xnorm = 0;
    for (i = 0; i < size; ++i) {
        fscanf(fp, "%lf\t", &temp);
        error += (temp - X[i]) * (temp - X[i]);
        Xnorm += X[i] * X[i];
    }
    error = sqrt(error);
    Xnorm = sqrt(Xnorm);
    printf("The relative error to the reference solution is %e\n", error / Xnorm);
    if (error / Xnorm <= TOL)
        printf("Congratulation!!! Your result is accepted!\n");
    else
        printf("Sorry, your result is wrong.\n");

    fclose(fp);
    DestroyVec(X);
    DestroyMat(Au, size);
    free(index);
    return 0;
}
