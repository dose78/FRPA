#include "harness.h"
#include "framework.h"
#include "CarmaProblem.h"

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
    std::string interleaving = argv[4];

    FILE *f = fopen("carma-double.csv","a");

    double *A, *B, *C;
    A = (double*) malloc(m * k * sizeof(double));
    B = (double*) malloc(k * n * sizeof(double));
    C = (double*) malloc(m * n * sizeof(double));
    initialize(m, k, n, A, B, C);
    CarmaProblem* problem = new CarmaProblem(m, k, m, k, n, m, A, B, C);

    // Time CARMA
    struct timeval start, end;
    gettimeofday(&start, NULL);
    Framework::solve(problem, interleaving); // warmup
    gettimeofday(&end, NULL);
    double seconds = (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
    double Gflop_s = 2e-9 * m * k * n / seconds;

    if (seconds < 0.02) {
        printf("WARNING: Matrix size may be too small to produce accurate timing data\n");
    }

    #ifdef DEBUG
        fprintf(f,"CARMA-DOUBLE,%d,%d,%d,%s,%f,%ld,%ld\n", m, k, n, interleaving.c_str(), Gflop_s, Memory::getMax(), Memory::getTotal());
        printf("CARMA-DOUBLE,%d,%d,%d,%s,%f,%ld,%ld\n", m, k, n, interleaving.c_str(), Gflop_s, Memory::getMax(), Memory::getTotal());
    #else
        fprintf(f,"CARMA-DOUBLE,%d,%d,%d,%s,%f\n", m, k, n, interleaving.c_str(), Gflop_s);
        printf("CARMA-DOUBLE,%d,%d,%d,%s,%f\n", m, k, n, interleaving.c_str(), Gflop_s);
    #endif

    // Housekeeping
    free(A);
    free(B);
    free(C);
    delete problem;
    fclose(f);
    return 0;
}
