#include "harness.h"
#include "framework.h"
#include "CholeskyProblem.h"

void initialize(double *A, int n) {
    srand48(time(NULL));
    for(int i = 0; i < n*n; i++) { A[i] = 2 * drand48() - 1; }
}

int main() {
    int n = 1024;

    double *A = (double*) malloc(n * n * sizeof(double));
    initialize(A, n);

    CholeskyProblem* problem = new CholeskyProblem(A, n, n);
    solve(problem);

    // TODO: Test for correctness

    free(A);
    delete problem;
}
