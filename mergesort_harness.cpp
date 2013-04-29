#include "harness.h"
#include "MergesortProblem.h"
#include "framework.h"

#define MIN_LENGTH 1
#define MAX_LENGTH 1000

void initialize(double *A, int length) {
    srand48(time(NULL));
    for(int i = 0; i < length; i++) A[i] = 10000 * drand48() - 1;
}

void print(double *A, int length) {
    for(int i = 0; i < length - 1; i++) printf("%.2f, ", A[i]);
    printf("%.2f\n", A[length - 1]);
}

int cmp_harness(const void *x, const void *y) {
    double xx = *(double*)x, yy = *(double*)y;
    if (xx < yy) return -1;
    if (xx > yy) return  1;
    return 0;
}

int main(int argc, char **argv) {
    srand48(time(NULL));
    FILE *f = fopen("mergesort.csv","a");

    // for (int length = MIN_LENGTH; length <= MAX_LENGTH; length++) {
    int length = 100000000; // dont try over 1 billion = 1000000000
    double *A = (double*) malloc(length * sizeof(double));
    double *B = (double*) malloc(length * sizeof(double));
    initialize(A, length);
    memcpy(B, A, length * sizeof(double));

    MergesortProblem* problem = new MergesortProblem(A, length);

    struct timeval start, end;
    gettimeofday(&start, NULL);
    solve(problem);
    gettimeofday(&end, NULL);
    double mergeTime = (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
    printf("MERGESORT,%d,%f\n", length, mergeTime);

    gettimeofday(&start, NULL);
    qsort(B, length, sizeof(double), cmp_harness);
    gettimeofday(&end, NULL);
    double builtInTime = (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
    printf("BUILT-IN,%d,%f\n", length, builtInTime);
    printf("SPEEDUP: %f\n\n", (builtInTime / mergeTime));

    fprintf(f,"%d,%f,%f,%f\n", length, mergeTime, builtInTime, (builtInTime / mergeTime));

    // check for correctness
    // for(int i = 0; i < length-1; i++) {
    //     if (A[i] > A[i+1]) {
    //         printf("FAILURE: array not sorted\n");
    //         exit(EXIT_FAILURE);
    //         break;
    //     }
    // }

    delete problem;
    free(A);
    free(B);
    // }

    fclose(f);
    return 0;
}
