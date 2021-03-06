#include "harness.h"
#include "framework.h"
#include "StrassenProblem.h"
// #include "StrassenNaiveProblem.h"

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
    std::string interleaving;
    if (argc > 4) {
        interleaving = argv[4];
    } else {
        interleaving = "";
    }

    FILE *f = fopen("strassen-double.csv","a");

    double *A, *B, *C;
    A = (double*) malloc(m * k * sizeof(double));
    B = (double*) malloc(k * n * sizeof(double));
    C = (double*) malloc(m * n * sizeof(double));
    initialize(m, k, n, A, B, C);
    StrassenProblem* problem = new StrassenProblem(m, k, n, A, m, B, k, C, m);
    // StrassenNaiveProblem* problem = new StrassenNaiveProblem(m, k, n, A, B, C);

    // Time multiplication
    struct timeval start, end;
    gettimeofday(&start, NULL);
    Framework::solve(problem, interleaving);
    gettimeofday(&end, NULL);
    double seconds = (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
    double Gflop_s = 2e-9 * m * k * n / seconds;

    if (seconds < 0.02) {
        printf("WARNING: Matrix size may be too small to produce accurate timing data\n");
    }

    #ifdef DEBUG
        fprintf(f,"STRASSEN-DOUBLE,%d,%d,%d,%s,%f,%ld,%ld\n", m, k, n, interleaving.c_str(), Gflop_s, Memory::getMax(), Memory::getTotal());
        printf("STRASSEN-DOUBLE,%d,%d,%d,%s,%f,%ld,%ld\n", m, k, n, interleaving.c_str(), Gflop_s, Memory::getMax(), Memory::getTotal());
    #else
        fprintf(f,"STRASSEN-DOUBLE,%d,%d,%d,%s,%f\n", m, k, n, interleaving.c_str(), Gflop_s);
        printf("STRASSEN-DOUBLE,%d,%d,%d,%s,%f\n", m, k, n, interleaving.c_str(), Gflop_s);
    #endif


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

    // Housekeeping
    free(A);
    free(B);
    free(C);
    delete problem;
    fclose(f);
    return 0;
}
