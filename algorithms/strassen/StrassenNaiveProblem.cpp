#include "StrassenNaiveProblem.h"
#include "debug.h"
#include "memory.h"

StrassenNaiveProblem::StrassenNaiveProblem(int m, int k, int n, double *A, double *B, double *C) {
    this->m = m;
    this->n = n;
    this->k = k;
    this->A = A;
    this->B = B;
    this->C = C;
}

void StrassenNaiveProblem::runBaseCase() {
    cblas_dgemm(CblasColMajor, CblasNoTrans, CblasNoTrans, m, n, k, 1, A, m, B, k, 0, C, m);
}

std::vector<Task*> StrassenNaiveProblem::split() {
    int T_m = m/2;
    int T_n = k/2;
    int S_m = k/2;
    int S_n = n/2;

    double *A11 = A;
    double *A21 = A + m/2;
    double *A12 = A + m*k/2;
    double *A22 = A + m*k/2 + m/2;

    double *B11 = B;
    double *B21 = B + k/2;
    double *B12 = B + k*n/2;
    double *B22 = B + k*n/2 + k/2;

    double *T[7], *S[7], *Q[7];
    for (int i = 0; i < 7; i++) {
        T[i] = (double*) malloc(T_m * T_n * sizeof(double));
        S[i] = (double*) malloc(S_m * S_n * sizeof(double));
        Q[i] = (double*) malloc(T_m * S_n * sizeof(double));
    }

    matrix_copy(T_m, T_n, T[0], T_m, A11, m);
    matrix_copy(T_m, T_n, T[1], T_m, A12, m);
    matrix_add(T_m, T_n, A21, m, A22, m, T[2], T_m);
    matrix_subtract(T_m, T_n, T[2], T_m, A11, m, T[3], T_m);
    matrix_subtract(T_m, T_n, A11, m, A21, m, T[4], T_m);
    matrix_subtract(T_m, T_n, A12, m, T[3], T_m, T[5], T_m);
    matrix_copy(T_m, T_n, T[6], T_m, A22, m);

    matrix_copy(S_m, S_n, S[0], S_m, B11, k);
    matrix_copy(S_m, S_n, S[1], S_m, B21, k);
    matrix_subtract(S_m, S_n, B12, k, B11, k, S[2], S_m);
    matrix_subtract(S_m, S_n, B22, k, S[2], S_m, S[3], S_m);
    matrix_subtract(S_m, S_n, B22, k, B12, k, S[4], S_m);
    matrix_copy(S_m, S_n, S[5], S_m, B22, k);
    matrix_subtract(S_m, S_n, S[3], S_m, B21, k, S[6], S_m);

    std::vector<Task*> tasks (7);
    for (int i = 0; i < 7; i++) {
        tasks[i] = new Task(new StrassenNaiveProblem(T_m, T_n, S_n, T[i], S[i], Q[i]));
    }
    return tasks;
}

void StrassenNaiveProblem::merge(std::vector<Problem*> problems) {
    double *C11 = C;
    double *C21 = C + m/2;
    double *C12 = C + m*n/2;
    double *C22 = C + m*n/2 + m/2;

    double *U1 = (double*) malloc(m/2 * n/2 * sizeof(double));
    double *U2 = (double*) malloc(m/2 * n/2 * sizeof(double));
    double *U3 = (double*) malloc(m/2 * n/2 * sizeof(double));

    double *Q[7];
    int counter = 0;
    for(std::vector<Problem*>::iterator problemIterator = problems.begin(); problemIterator != problems.end(); problemIterator++) {
        StrassenNaiveProblem *problem = (StrassenNaiveProblem*)*problemIterator;
        Q[counter++] = (double*)problem->C;
    }

    matrix_add(m/2, n/2, Q[0], m/2, Q[3], m/2, U1, m/2);
    matrix_add(m/2, n/2, U1, m/2, Q[4], m/2, U2, m/2);
    matrix_add(m/2, n/2, U1, m/2, Q[2], m/2, U3, m/2);

    matrix_add(m/2, n/2, Q[0], m/2, Q[1], m/2, C11, m);
    matrix_add(m/2, n/2, U3, m/2, Q[5], m/2, C12, m);
    matrix_subtract(m/2, n/2, U2, m/2, Q[6], m/2, C21, m);
    matrix_add(m/2, n/2, U2, m/2, Q[2], m/2, C22, m);

    free(U1);
    free(U2);
    free(U3);
    for(std::vector<Problem*>::iterator problemIterator = problems.begin(); problemIterator != problems.end(); problemIterator++) {
        StrassenNaiveProblem *problem = (StrassenNaiveProblem*)*problemIterator;
        free(problem->A);
        free(problem->B);
        free(problem->C);
    }
}

// C <- A + B
void StrassenNaiveProblem::matrix_add(int m, int n, double *A, int lda, double *B, int ldb, double *C, int ldc) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            C[i*ldc + j] = A[i*lda + j] + B[i*ldb + j];
        }
    }
}

// C <- A - B
void StrassenNaiveProblem::matrix_subtract(int m, int n, double *A, int lda, double *B, int ldb, double *C, int ldc) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            C[i*ldc + j] = A[i*lda + j] - B[i*ldb + j];
        }
    }
}

// Copy B into A
void StrassenNaiveProblem::matrix_copy(int m, int n, double *A, int lda, double *B, int ldb) {
    for (int i = 0; i < n; i++) {
        memcpy(A + i*lda, B + i*ldb, m * sizeof(double));
    }
}
