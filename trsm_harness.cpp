#include "TrsmProblem.h"
#include "framework.h"
#include "harness.h"

void initialize(int n, double *X, double *T, double *X2, double *T2) {
    srand48(time(NULL));
    for(int i = 0; i < n*n; i++) { X[i] = X2[i] = 2 * drand48() - 1;}
    for(int i = 0; i < n*n; i++) { T[i] = T2[i] = 2 * drand48() - 1;}
}

int main() {
    
    for(int n = 64; n <= 16384; n *= 2) {

        double *X = (double*) malloc(n * n * sizeof(double));
        double *T = (double*) malloc(n * n * sizeof(double));
        double *X2 = (double*) malloc(n * n * sizeof(double));
        double *T2 = (double*) malloc(n * n * sizeof(double));
        initialize(n, X, T, X2, T2);
        TrsmProblem* problem = new TrsmProblem(X, T, n, n);

        FILE *f = fopen("trsm.csv","a");

        // Timing
        struct timeval start, end;
        gettimeofday(&start, NULL);

        solve(problem);
        // cblas_dtrsm(CblasColMajor, CblasRight, CblasLower, CblasTrans, CblasNonUnit, n, n, 1.0, T2, n, X2, n);

        gettimeofday(&end, NULL);
        double seconds = (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);

        fprintf(f,"TRSM: %d,%f\n", n, seconds);
        printf("TRSM: %d,%f\n", n, seconds);

        // Correctness
        // cblas_dtrsm(CblasColMajor, CblasRight, CblasLower, CblasTrans, CblasNonUnit, n, n, 1.0, T2, n, X2, n);

        // for(int i = 0; i < n*n; i++) {
        //     if ((fabs(X[i] - X2[i]) / X[i]) > .00000000001) {
        //         printf("ERROR: %f\n", fabs((X[i] - X2[i]) / X[i]));
        //     }
        // }

        free(X);
        free(T);
        free(X2);
        free(T2);
        delete problem;
    }
}
