#include "harness.h"
#include "CarmaProblem.h"
#include "framework.h"

#define NUM_SMALL_MATRICES_MAX 5000

void clearCache(double *F) {
    double sum = 0;
    for (int i = 0; i < 12500000; i++) {
        sum += F[i];
    }
    if (sum == 0.1) { // Prevent the compiler from optimizing this away
        printf("sum = %f\n", sum);
    }
}

void initialize(int m, int k, int n, double* A, double* B, double* C) {
    for(int i = 0; i < m*k; i++) A[i] = 2 * drand48() - 1;
    for(int i = 0; i < k*n; i++) B[i] = 2 * drand48() - 1;
    for(int i = 0; i < m*n; i++) C[i] = 2 * drand48() - 1;
}

int guess_num_matrices(int m, int k, int n) {
    double *A[NUM_SMALL_MATRICES_MAX], *B[NUM_SMALL_MATRICES_MAX], *C[NUM_SMALL_MATRICES_MAX];
    double *cacheClearer = (double*) malloc(100000000); //L3 cahce is less than 100MB
    CarmaProblem** problems = (CarmaProblem**) malloc(NUM_SMALL_MATRICES_MAX * sizeof(CarmaProblem*));
    int num_matrices, i, previous_trial = 0;
    struct timeval start, end;
    for(i = 0; i < 12500000; i++) cacheClearer[i] = 2 * drand48() - 1;

    for (num_matrices = 1; num_matrices < NUM_SMALL_MATRICES_MAX; num_matrices *= 2) {
        for (int i = previous_trial; i < num_matrices; i++) {
            A[i] = (double*) malloc(m * k * sizeof(double));
            B[i] = (double*) malloc(k * n * sizeof(double));
            C[i] = (double*) malloc(m * n * sizeof(double));
            initialize(m, k, n, A[i], B[i], C[i]);
            problems[i] = new CarmaProblem(m, k, m, k, n, m, A[i], B[i], C[i]);
        }
        previous_trial = num_matrices;

        clearCache(cacheClearer); // clear cache
        gettimeofday(&start, NULL);
        for (int i = 0; i < num_matrices; i++) {
            solve(problems[i]);
            // cblas_dgemm(CblasColMajor,CblasNoTrans,CblasNoTrans, m,n,k, -1, A[0],m, B[0],k, 1, C[0],m);
        }
        gettimeofday(&end, NULL);
        double seconds = (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
        // printf("%i matrices take %f seconds.\n", num_matrices, seconds);
        if (seconds > 0.2) {
            break;
        }
    }

    for (int i = 0; i < num_matrices; i++) {
        free(A[i]);
        free(B[i]);
        free(C[i]);
        delete problems[i];
    }
    free(problems);
    return num_matrices;
}

int main(int argc, char **argv) {
    srand48(time(NULL));
    double *cacheClearer = (double*) malloc(100000000); // L3 cache is less than 100MB
    for(int i = 0; i < 12500000; i++) cacheClearer[i] = 2 * drand48() - 1;

    int m = atoi(argv[1]);
    int k = atoi(argv[2]);
    int n = atoi(argv[3]);

    FILE *f = fopen("carma.csv","a");

    // discover how many multiplies are needed
    int num_matrices = guess_num_matrices(m, k, n);
    // printf("Num matrices required: %d\n", num_matrices);

    CarmaProblem** problems = (CarmaProblem**) malloc(num_matrices * sizeof(CarmaProblem*));
    double *A[num_matrices], *B[num_matrices], *C[num_matrices];
    for (int i = 0; i < num_matrices; i++) {
        A[i] = (double*) malloc(m * k * sizeof(double));
        B[i] = (double*) malloc(k * n * sizeof(double));
        C[i] = (double*) malloc(m * n * sizeof(double));
        initialize(m, k, n, A[i], B[i], C[i]);
        problems[i] = new CarmaProblem(m, k, m, k, n, m, A[i], B[i], C[i]);
    }

    // Time CARMA
    struct timeval start, end;
    solve(problems[0]); // warmup
    clearCache(cacheClearer); // clear cache
    gettimeofday(&start, NULL);
    for (int i = 0; i < num_matrices; i++) {
        solve(problems[i]);
    }
    gettimeofday(&end, NULL);
    double seconds = (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
    double Gflop_s = num_matrices * 2e-9 * m * k * n / seconds;
    if (seconds < 0.02) {
        printf("WARNING: Matrix size may be too small to produce accurate timing data\n");
    }
    fprintf(f,"CARMA: %d,%d,%d,%f\n", m, k, n, Gflop_s);
    printf("CARMA: %d,%d,%d,%f\n", m, k, n, Gflop_s);

    // check for correctness
    // memset(C[0], 0, sizeof(double) * m * n); //if commented, this tests C = A*B instead of C += A*B or C = A*B
    // solve(problems[0]);
    // cblas_dgemm(CblasColMajor,CblasNoTrans,CblasNoTrans, m,n,k, -1, A[0],m, B[0],k, 1, C[0],m);
    // for(int i = 0; i < m*k; i++) A[0][i] = fabs( A[0][i] );
    // for(int i = 0; i < k*n; i++) B[0][i] = fabs( B[0][i] );
    // for(int i = 0; i < m*n; i++) C[0][i] = fabs( C[0][i] );
    // cblas_dgemm(CblasColMajor,CblasNoTrans,CblasNoTrans, m,n,k, -3.0*DBL_EPSILON*n, A[0],m, B[0],k, 1, C[0],m);
    // for(int i = 0; i < m*n; i++) {
    //     if(C[0][i] > 0) {
    //         printf("FAILURE: error in matrix multiply exceeds an acceptable margin\n");
    //         return -1;
    //     }
    // }

    // Housekeeping
    for (int i=0; i<num_matrices; i++) free(A[i]);
    for (int i=0; i<num_matrices; i++) free(B[i]);
    for (int i=0; i<num_matrices; i++) free(C[i]);
    for (int i=0; i<num_matrices; i++) delete problems[i];
    free(cacheClearer);
    free(problems);
    fclose(f);
    return 0;
}
