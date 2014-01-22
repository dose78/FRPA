#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <mkl.h>

void multiply(int m, int k, int n, double *A, int lda, double *B, int ldb, double *C, int ldc, char* interleaving, int interleaving_length);

void initialize(int m, int k, int n, double* A, double* B, double* C) {
    for(int i = 0; i < m*k; i++) A[i] = 2 * drand48() - 1;
    for(int i = 0; i < k*n; i++) B[i] = 2 * drand48() - 1;
    for(int i = 0; i < m*n; i++) C[i] = 2 * drand48() - 1;
}

int main(int argc, char **argv) {
    srand48(time(NULL));
    int m = atoi(argv[1]);
    int k = atoi(argv[2]);
    int n = atoi(argv[3]);
    char* interleaving = argv[4];
    int interleaving_length = atoi(argv[5]);

    double *A, *B, *C;
    A = (double*) malloc(m * k * sizeof(double));
    B = (double*) malloc(k * n * sizeof(double));
    C = (double*) malloc(m * n * sizeof(double));
    initialize(m, k, n, A, B, C);

    struct timeval start, end;
    gettimeofday(&start, NULL);
    multiply(m, k, n, A, m, B, k, C, m, interleaving, interleaving_length);
    gettimeofday(&end, NULL);
    double seconds = (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
    double Gflop_s = 2e-9 * m * k * n / seconds;

    if (seconds < 0.01) {
        printf("WARNING: Matrix size may be too small to produce accurate timing data\n");
    }

    printf("STRASSEN,%d,%d,%d,%s,%f\n", m, k, n, interleaving, Gflop_s);

    // check for correctness
    // cblas_dgemm(CblasColMajor,CblasNoTrans,CblasNoTrans, m,n,k, -1, A,m, B,k, 1, C,m);
    // for(int i = 0; i < m*k; i++) A[i] = fabs( A[i] );
    // for(int i = 0; i < k*n; i++) B[i] = fabs( B[i] );
    // for(int i = 0; i < m*n; i++) C[i] = fabs( C[i] );
    // cblas_dgemm(CblasColMajor,CblasNoTrans,CblasNoTrans, m,n,k, -3.0*DBL_EPSILON*n, A,m, B,k, 1, C,m);
    // for(int i = 0; i < m*n; i++) {
    //     if(C[i] > 0) {
    //         printf("FAILURE: error in matrix multiply exceeds an acceptable margin\n");
    //         return -1;
    //     }
    // }
    // printf("test passed\n");

    // Housekeeping
    free(A);
    free(B);
    free(C);
    return 0;
}
