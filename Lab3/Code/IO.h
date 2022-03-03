/*
Header for the Lab 3 IO functions
*/
#ifndef LAB3_H_INCLUDE
#define LAB3_H_INCLUDE


#ifdef __cplusplus
extern "C" {
#endif

int LoadInput(double ***A, int *size);
int SaveOutput(double* x, int size, double time);

double** CreateMat(int NumRow, int NumCol);
int DestroyMat(double** A, int NumRow);
int PrintMat(double** A, int NumRow, int NumCol);
double* CreateVec(int size);
int PrintVec(double* b, int size);
int DestroyVec(double* b);

#ifdef __cplusplus
};
#endif

#endif