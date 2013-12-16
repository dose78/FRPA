#include "harness.h"
#include "framework.h"
#include "CholeskyProblem.h"

void initialize(double *A, double *A2, int n) {
    srand48(time(NULL));
    for(int i = 0; i < n*n; i++) { A[i] = A2[i] = 2 * drand48() - 1; }
    for(int i = 0; i < n; i++) { A[i + n*i] += 2*n; }
    for(int i = 0; i < n; i++) { A2[i + n*i] += 2*n; }
}

int main(int argc, char **argv) {
    std::string interleaving;
    if (argc > 1) {
        interleaving = argv[1];
    } else {
        interleaving = "";
    }

    FILE *f = fopen("cholesky.csv","a");
    int n = 1024;

    double *A = (double*) malloc(n * n * sizeof(double));
    double *A2 = (double*) malloc(n * n * sizeof(double));
    initialize(A, A2, n);
    CholeskyProblem* problem = new CholeskyProblem(A, n, n);

    // Timing
    struct timeval start, end;
    gettimeofday(&start, NULL);
    Framework::solve(problem, interleaving);
    gettimeofday(&end, NULL);
    double seconds = (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);

    #ifdef DEBUG
        fprintf(f,"CHOLESKY,%d,%s,%f,%ld,%ld\n", n, interleaving.c_str(), seconds, Memory::getMax(), Memory::getTotal());
        printf("CHOLESKY,%d,%s,%f,%ld,%ld\n", n, interleaving.c_str(), seconds, Memory::getMax(), Memory::getTotal());
    #else
        fprintf(f,"CHOLESKY,%d,%s,%f\n", n, interleaving.c_str(), seconds);
        printf("CHOLESKY,%d,%s,%f\n", n, interleaving.c_str(), seconds);
    #endif

    int info;
    char *Lc = "L";
    dpotrf(Lc, &n, A2, &n, &info);
    if (info != 0) {
        printf("info: %d\n", info);
    }
    for(int i = 0; i < n*n; i++) {
        if ((fabs(A[i] - A2[i]) / A[i]) > .01) {
            // printf("A = %f | A2 = %f\n", A[i], A2[i]);
            printf("ERROR: %f\n", fabs((A[i] - A2[i]) / A[i]));
            exit(EXIT_FAILURE);
        }
        // printf("A = %f | A2 = %f\n", A[i], A2[i]);
    }

    free(A);
    free(A2);
    delete problem;
    fclose(f);
}
