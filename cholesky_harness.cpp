#include "harness.h"
#include "framework.h"
#include "CholeskyProblem.h"

void initialize(double *A, double *A2, int n) {
    srand48(time(NULL));
    for(int i = 0; i < n*n; i++) { A[i] = A2[i] = 2 * drand48() - 1; }
}

int main() {
    int n = 1024;

    double *A = (double*) malloc(n * n * sizeof(double));
    double *A2 = (double*) malloc(n * n * sizeof(double));
    initialize(A, A2, n);

    CholeskyProblem* problem = new CholeskyProblem(A, n, n);
    solve(problem);

    int info;
    char *Lc = "L";
    dpotrf(Lc, &n, A2, &n, &info);

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
}
