#include "IO.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int n;
    int i, j, k;
    FILE *fp;
    int temp, flag = 1;
    int **A;
    int **B;
    int **C;

    LoadInput(&A, &B, &n);
    C = malloc(n * sizeof(int *));
    for (i = 0; i < n; i++)
        C[i] = malloc(n * sizeof(int));

    // Calculating
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++) {
            C[i][j] = 0;
            for (k = 0; k < n; k++)
                C[i][j] += A[i][k] * B[k][j];
        }

    // Testing
    if ((fp = fopen("data_output", "r")) == NULL) {
        printf("Fail to load the output data.\n");
        return 1;
    }
    fscanf(fp, "%d\n\n", &temp);
    if (temp != n) {
        printf("Error, the output dimension did not match the input.\n");
        return 2;
    }
    for (i = 0; i < n && flag == 1; i++)
        for (j = 0; j < n && flag == 1; j++) {
            fscanf(fp, "%d", &temp);
            if (temp != C[i][j])
                flag = 0;
        }
    if (flag == 1)
        printf("The result is correct!\n");
    else
        printf("The result is wrong.\n");

    for (i = 0; i < n; i++) {
        free(A[i]);
        free(B[i]);
        free(C[i]);
    }
    free(A);
    free(B);
    free(C);
    return 0;
}
