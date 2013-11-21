#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <mkl.h>
#include <memory.h>
#include <string>

void initialize(int m, int k, int n, float* A, float* B, float* C) {
    for(int i = 0; i < m*k; i++) A[i] = 2 * drand48() - 1;
    for(int i = 0; i < k*n; i++) B[i] = 2 * drand48() - 1;
    for(int i = 0; i < m*n; i++) C[i] = 2 * drand48() - 1;
}

int main(int argc, char **argv) {
    srand48(time(NULL));

    int m = atoi(argv[1]);
    int k = atoi(argv[2]);
    int n = atoi(argv[3]);
    // std::string interleaving = argv[4];
    char* outfile = argv[4];

    FILE *f = fopen(outfile,"a");

    float *A, *B, *C;
    A = (float*) malloc(m * k * sizeof(float));
    B = (float*) malloc(k * n * sizeof(float));
    C = (float*) malloc(m * n * sizeof(float));
    initialize(m, k, n, A, B, C);

    // Time multiplication
    struct timeval start, end;
    gettimeofday(&start, NULL);
    cblas_sgemm(CblasColMajor,CblasNoTrans,CblasNoTrans, m,n,k, 1, A,m, B,k, 1, C,m);
    gettimeofday(&end, NULL);
    double seconds = (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
    double Gflop_s = 2e-9 * m * k * n / seconds;
    fprintf(f,"MKL-SINGLE,%d,%d,%d,%s,%f\n", m, k, n, "MKL", Gflop_s);
    printf("MKL-SINGLE,%d,%d,%d,%s,%f\n", m, k, n, "MKL", Gflop_s);

    // Housekeeping
    free(A);
    free(B);
    free(C);
    fclose(f);
    return 0;
}
