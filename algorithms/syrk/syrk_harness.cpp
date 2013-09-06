#include "harness.h"
#include "framework.h"
#include "SyrkProblem.h"

void initialize(double *C, double *A, double *C2, double *A2, int n) {
    srand48(time(NULL));
    for(int i = 0; i < n*n; i++) { A[i] = A2[i] = 2 * drand48() - 1;}
    for(int i = 0; i < n*n; i++) { C[i] = C2[i] = 2 * drand48() - 1;}
}

int main(int argc, char **argv) {
    FILE *f = fopen("syrk.csv","a");
    int n = 1024;

    double *C = (double*) malloc(n * n * sizeof(double));
    double *A = (double*) malloc(n * n * sizeof(double));
    double *C2 = (double*) malloc(n * n * sizeof(double));
    double *A2 = (double*) malloc(n * n * sizeof(double));
    initialize(C, A, C2, A2, n);
    SyrkProblem* problem = new SyrkProblem(C, A, n, n, n);

    struct timeval start, end;
    gettimeofday(&start, NULL);
    solve(problem);
    gettimeofday(&end, NULL);
    double seconds = (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
    fprintf(f,"SYRK: %d,%f\n", n, seconds);
    printf("SYRK: %d,%f\n", n, seconds);

    // Correctness
    cblas_dsyrk(CblasColMajor, CblasLower, CblasNoTrans, n, n, -1.0, A2, n, 1.0, C2, n);
    // char *Lc = "L";
    // char *Nc = "N";
    // const double one = 1.0;
    // const double negone = -1.0;
    // dsyrk(Lc, Nc, &n, &n, &negone, A2, &n, &one, C2, &n);
    for(int i = 0; i < n*n; i++) {
        if ((fabs(C[i] - C2[i]) / C[i]) > .0000000001) {
            printf("ERROR: %f\n", fabs((C[i] - C2[i]) / C[i]));
            exit(EXIT_FAILURE);
        }
    }
#ifdef DEBUG
	printf("memory: %d bytes\n", Memory::current);
	printf("max: %d bytes\n", Memory::max);
#endif
    free(C);
    free(A);
    free(C2);
    free(A2);
    delete problem;
    fclose(f);
}
