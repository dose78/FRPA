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
    FILE *f = fopen("cholesky.csv","a");
    int n = 1024;

    double *A = (double*) malloc(n * n * sizeof(double));
    double *A2 = (double*) malloc(n * n * sizeof(double));
    initialize(A, A2, n);
    CholeskyProblem* problem = new CholeskyProblem(A, n, n);

    // Timing
    struct timeval start, end;
    gettimeofday(&start, NULL);
    Framework::solve(problem);
    gettimeofday(&end, NULL);
    double seconds = (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
    fprintf(f,"Cholesky: %d,%f\n", n, seconds);
    printf("Cholesky: %d,%f\n", n, seconds);

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
