#include "harness.h"
#include "framework.h"
#include "TrsmProblem.h"

void initialize(int n, double *X, double *T, double *X_test, double *T_test) {
    srand48(time(NULL));
    for(int i = 0; i < n*n; i++) { X[i] = X_test[i] = 2 * drand48() - 1;}
    for(int i = 0; i < n*n; i++) { T[i] = T_test[i] = 2 * drand48() - 1;}
}

int main(int argc, char **argv) {
    FILE *f = fopen("trsm.csv","a");

    for(int n = 64; n <= 16384; n *= 2) {
        double *X = (double*) malloc(n * n * sizeof(double));
        double *T = (double*) malloc(n * n * sizeof(double));
        double *X_test = (double*) malloc(n * n * sizeof(double));
        double *T_test = (double*) malloc(n * n * sizeof(double));
        initialize(n, X, T, X_test, T_test);
        TrsmProblem* problem = new TrsmProblem(X, T, n, n);

        // Timing
        struct timeval start, end;
        gettimeofday(&start, NULL);
        Framework::solve(problem);
        gettimeofday(&end, NULL);
        double seconds = (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
        fprintf(f,"TRSM: %d,%f\n", n, seconds);
        printf("TRSM: %d,%f\n", n, seconds);

        // Correctness
        cblas_dtrsm(CblasColMajor, CblasRight, CblasLower, CblasTrans, CblasNonUnit, n, n, 1.0, T_test, n, X_test, n);
        for(int i = 0; i < n*n; i++) {
            if ((fabs(X[i] - X_test[i]) / X[i]) > .00000000001) {
                printf("ERROR: %f\n", fabs((X[i] - X_test[i]) / X[i]));
            }
        }

        free(X);
        free(T);
        free(X_test);
        free(T_test);
        delete problem;
    }
    fclose(f);
}
