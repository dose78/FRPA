#include "TrsmProblem.h"
#include "MultProblem.h"
#include "Framework.h"

void initialize(int n, double *X, double *T) {
    srand48(time(NULL));
    int i;
    for(i = 0; i < n*n; i++) { X[i] = 2 * drand48() - 1;}
    for(i = 0; i < n*n; i++) { T[i] = 2 * drand48() - 1;}
}

int main() {
    int n = 64;
    double *X = (double*) malloc(n * n * sizeof(double));
    double *T = (double*) malloc(n * n * sizeof(double));
    TrsmProblem* problem = new TrsmProblem(X, T, n, n);

    solve(problem);

    free(X);
    free(T);
    delete problem;
}
