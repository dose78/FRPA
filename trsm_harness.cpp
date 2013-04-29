#include "TrsmProblem.h"
#include "framework.h"
#include "harness.h"

void initialize(int n, double *X, double *T, double *X2, double *T2) {
    srand48(time(NULL));
    // memset(T, 0.0, sizeof(double)*n*n);
    // memset(T2, 0.0, sizeof(double)*n*n);
    for(int i = 0; i < n*n; i++) { X[i] = X2[i] = 2 * drand48() - 1;}
    for(int i = 0; i < n*n; i++) { T[i] = T2[i] = 2 * drand48() - 1;}
}

int main() {
    int n = 1024;

    double *X = (double*) malloc(n * n * sizeof(double));
    double *T = (double*) malloc(n * n * sizeof(double));
    double *X2 = (double*) malloc(n * n * sizeof(double));
    double *T2 = (double*) malloc(n * n * sizeof(double));
    initialize(n, X, T, X2, T2);

    TrsmProblem* problem = new TrsmProblem(X, T, n, n);
    solve(problem);

    cblas_dtrsm(CblasColMajor, CblasRight, CblasLower, CblasTrans, CblasNonUnit, n, n, 1.0, T2, n, X2, n);

    for(int i = 0; i < n*n; i++) {
        if ((fabs(X[i] - X2[i]) / X[i]) > .001) {
            // printf("X = %f | X2 = %f\n", X[i], X2[i]);
            printf("ERROR: %f\n", fabs((X[i] - X2[i]) / X[i]));
            // printf("FAILURE\n");
            // exit(EXIT_FAILURE);
        }
        // printf("X = %f | X2 = %f\n", X[i], X2[i]);
    }

    free(X);
    free(T);
    free(X2);
    free(T2);
    delete problem;
}
