#include "StrassenSingleNaiveProblem.h"
#include "debug.h"
#include "memory.h"

StrassenSingleNaiveProblem::StrassenSingleNaiveProblem(int m, int k, int n, float *A, float *B, float *C) {
    this->m = m;
    this->n = n;
    this->k = k;
    this->A = A;
    this->B = B;
    this->C = C;
}

void StrassenSingleNaiveProblem::runBaseCase() {
    cblas_sgemm(CblasColMajor, CblasNoTrans, CblasNoTrans, m, n, k, 1, A, m, B, k, 0, C, m);
}

std::vector<Task*> StrassenSingleNaiveProblem::split() {
    int T_m = m/2;
    int T_n = k/2;
    int S_m = k/2;
    int S_n = n/2;

    float *A11 = A;
    float *A21 = A + m/2;
    float *A12 = A + m*k/2;
    float *A22 = A + m*k/2 + m/2;

    float *B11 = B;
    float *B21 = B + k/2;
    float *B12 = B + k*n/2;
    float *B22 = B + k*n/2 + k/2;

    float *T[7], *S[7], *Q[7];
    for (int i = 0; i < 7; i++) {
        T[i] = (float*) malloc(T_m * T_n * sizeof(float));
        S[i] = (float*) malloc(S_m * S_n * sizeof(float));
        Q[i] = (float*) malloc(T_m * S_n * sizeof(float));
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
        tasks[i] = new Task(new StrassenSingleNaiveProblem(T_m, T_n, S_n, T[i], S[i], Q[i]));
    }
    return tasks;
}

void StrassenSingleNaiveProblem::merge(std::vector<Problem*> problems) {
    float *C11 = C;
    float *C21 = C + m/2;
    float *C12 = C + m*n/2;
    float *C22 = C + m*n/2 + m/2;

    float *U1 = (float*) malloc(m/2 * n/2 * sizeof(float));
    float *U2 = (float*) malloc(m/2 * n/2 * sizeof(float));
    float *U3 = (float*) malloc(m/2 * n/2 * sizeof(float));

    float *Q[7];
    int counter = 0;
    for(std::vector<Problem*>::iterator problemIterator = problems.begin(); problemIterator != problems.end(); problemIterator++) {
        StrassenSingleNaiveProblem *problem = (StrassenSingleNaiveProblem*)*problemIterator;
        Q[counter++] = (float*)problem->C;
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
        StrassenSingleNaiveProblem *problem = (StrassenSingleNaiveProblem*)*problemIterator;
        free(problem->A);
        free(problem->B);
        free(problem->C);
    }
}

// C <- A + B
void StrassenSingleNaiveProblem::matrix_add(int m, int n, float *A, int lda, float *B, int ldb, float *C, int ldc) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            C[i*ldc + j] = A[i*lda + j] + B[i*ldb + j];
        }
    }
}

// C <- A - B
void StrassenSingleNaiveProblem::matrix_subtract(int m, int n, float *A, int lda, float *B, int ldb, float *C, int ldc) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            C[i*ldc + j] = A[i*lda + j] - B[i*ldb + j];
        }
    }
}

// Copy B into A
void StrassenSingleNaiveProblem::matrix_copy(int m, int n, float *A, int lda, float *B, int ldb) {
    for (int i = 0; i < n; i++) {
        memcpy(A + i*lda, B + i*ldb, m * sizeof(float));
    }
}
