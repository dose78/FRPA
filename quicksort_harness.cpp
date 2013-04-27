#include "harness.h"
#include "QuicksortProblem.h"
#include "framework.h"

#define MIN_LENGTH 1
#define MAX_LENGTH 1000

void initialize(double *A, int length) {
    srand48(time(NULL));
    for(int i = 0; i < length; i++) A[i] = 100 * drand48() - 1;
}

void print(double *A, int length) {
    for(int i = 0; i < length - 1; i++) printf("%.2f, ", A[i]);
    printf("%.2f\n", A[length - 1]);
}

int main(int argc, char **argv) {
    srand48(time(NULL));
    for (int length = MIN_LENGTH; length <= MAX_LENGTH; length++) {
        double *A = (double*) malloc(length * sizeof(double));
        initialize(A, length);

        QuicksortProblem* problem = new QuicksortProblem(A, length);
        solve(problem);
        delete problem;

        // check for correctness
        for(int i = 0; i < length-1; i++) {
            if (A[i] > A[i+1]) {
                printf("FAILURE: array not sorted\n");
                exit(EXIT_FAILURE);
                break;
            }
        }
        free(A);
    }
    printf("SUCCESS\n");
    return 0;
}
