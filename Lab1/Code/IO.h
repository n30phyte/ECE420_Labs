#pragma once
#ifdef __cplusplus
extern "C" {
#endif

int LoadInput(int ***A, int ***B, int *n);
int SaveOutput(int **C, int *n, double time);

#ifdef __cplusplus
};
#endif