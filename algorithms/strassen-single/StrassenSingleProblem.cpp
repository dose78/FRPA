#include "StrassenSingleProblem.h"
#include "memorytracking.h"

StrassenSingleProblem::StrassenSingleProblem(int m, int k, int n, float *A, int lda, float *B, int ldb, float *C, int ldc) {
    this->m = m;
    this->n = n;
    this->k = k;
    this->A = A;
    this->lda = lda;
    this->B = B;
    this->ldb = ldb;
    this->C = C;
    this->ldc = ldc;
}

void StrassenSingleProblem::runBaseCase() {
    cblas_sgemm(CblasColMajor, CblasNoTrans, CblasNoTrans, m, n, k, 1, A, lda, B, ldb, 0, C, ldc);
}

std::vector<Task*> StrassenSingleProblem::split() {
    int T_m = m/2;
    int T_n = k/2;
    int S_m = k/2;
    int S_n = n/2;

    float *A11 = A;
    float *A21 = A + m/2;
    float *A12 = A + lda*k/2;
    float *A22 = A + lda*k/2 + m/2;

    float *B11 = B;
    float *B21 = B + k/2;
    float *B12 = B + ldb*n/2;
    float *B22 = B + ldb*n/2 + k/2;

    float *C11 = C;
    float *C21 = C + m/2;
    float *C12 = C + ldc*n/2;
    float *C22 = C + ldc*n/2 + m/2;

    float *T0 = A11;
    float *T1 = A12;
    float *T2 = (float*) malloc(T_m * T_n * sizeof(float));
    float *T3 = (float*) malloc(T_m * T_n * sizeof(float));
    float *T4 = (float*) malloc(T_m * T_n * sizeof(float));
    float *T5 = (float*) malloc(T_m * T_n * sizeof(float));
    float *T6 = A22;

    float *S0 = B11;
    float *S1 = B21;
    float *S2 = (float*) malloc(S_m * S_n * sizeof(float));
    float *S3 = (float*) malloc(S_m * S_n * sizeof(float));
    float *S4 = (float*) malloc(S_m * S_n * sizeof(float));
    float *S5 = B22;
    float *S6 = (float*) malloc(S_m * S_n * sizeof(float));

    float *Q0 = C11;
    float *Q1 = (float*) malloc(T_m * S_n * sizeof(float));
    float *Q2 = C22;
    float *Q3 = C12;
    float *Q4 = C21;
    float *Q5 = (float*) malloc(T_m * S_n * sizeof(float));
    float *Q6 = (float*) malloc(T_m * S_n * sizeof(float));

    matrix_add(T_m, T_n, A21, lda, A22, lda, T2, T_m);
    matrix_subtract(T_m, T_n, T2, T_m, A11, lda, T3, T_m);
    matrix_subtract(T_m, T_n, A11, lda, A21, lda, T4, T_m);
    matrix_subtract(T_m, T_n, A12, lda, T3, T_m, T5, T_m);

    matrix_subtract(S_m, S_n, B12, ldb, B11, ldb, S2, S_m);
    matrix_subtract(S_m, S_n, B22, ldb, S2, S_m, S3, S_m);
    matrix_subtract(S_m, S_n, B22, ldb, B12, ldb, S4, S_m);
    matrix_subtract(S_m, S_n, S3, S_m, B21, ldb, S6, S_m);

    std::vector<Task*> tasks (7);
    tasks[0] = new Task(new StrassenSingleProblem(T_m, T_n, S_n, T0, lda, S0, ldb, Q0, ldc));
    tasks[1] = new Task(new StrassenSingleProblem(T_m, T_n, S_n, T1, lda, S1, ldb, Q1, T_m));
    tasks[2] = new Task(new StrassenSingleProblem(T_m, T_n, S_n, T2, T_m, S2, S_m, Q2, ldc));
    tasks[3] = new Task(new StrassenSingleProblem(T_m, T_n, S_n, T3, T_m, S3, S_m, Q3, ldc));
    tasks[4] = new Task(new StrassenSingleProblem(T_m, T_n, S_n, T4, T_m, S4, S_m, Q4, ldc));
    tasks[5] = new Task(new StrassenSingleProblem(T_m, T_n, S_n, T5, T_m, S5, ldb, Q5, T_m));
    tasks[6] = new Task(new StrassenSingleProblem(T_m, T_n, S_n, T6, lda, S6, S_m, Q6, T_m));
    return tasks;
}

void StrassenSingleProblem::merge(std::vector<Problem*> problems) {
    float *C11 = C;
    float *C21 = C + m/2;
    float *C12 = C + ldc*n/2;
    float *C22 = C + ldc*n/2 + m/2;

    float *Q[7];
    int counter = 0;
    for(std::vector<Problem*>::iterator problemIterator = problems.begin(); problemIterator != problems.end(); problemIterator++) {
        StrassenSingleProblem *problem = (StrassenSingleProblem*)*problemIterator;
        Q[counter++] = (float*) problem->C;
    }

    matrix_add_inplace(m/2, n/2, C11, ldc, C12, ldc);
    matrix_add_inplace(m/2, n/2, C12, ldc, C21, ldc);
    matrix_add_inplace(m/2, n/2, C22, ldc, C12, ldc);
    matrix_add_inplace(m/2, n/2, C21, ldc, C22, ldc);
    matrix_subtract_inplace(m/2, n/2, Q[6], m/2, C21, ldc);
    matrix_add_inplace(m/2, n/2, Q[5], m/2, C12, ldc);
    matrix_add_inplace(m/2, n/2, Q[1], m/2, C11, ldc);

    for(std::vector<Problem*>::iterator problemIterator = problems.begin(); problemIterator != problems.end(); problemIterator++) {
        StrassenSingleProblem *problem = (StrassenSingleProblem*)*problemIterator;
        if (problem->lda == m/2) {
            free(problem->A);
        }
        if (problem->ldb == k/2) {
            free(problem->B);
        }
        if (problem->ldc == m/2) {
            free(problem->C);
        }
    }
}

// C <- A + B
void StrassenSingleProblem::matrix_add(int m, int n, float *A, int lda, float *B, int ldb, float *C, int ldc) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            C[i*ldc + j] = A[i*lda + j] + B[i*ldb + j];
        }
    }
}

// B <- A + B
void StrassenSingleProblem::matrix_add_inplace(int m, int n, float *A, int lda, float *B, int ldb) {
    for (int i = 0; i < n; i++) {
        cblas_saxpy(m, 1, A + i*lda, 1, B + i*ldb, 1);
    }
}

// C <- A - B
void StrassenSingleProblem::matrix_subtract(int m, int n, float *A, int lda, float *B, int ldb, float *C, int ldc) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            C[i*ldc + j] = A[i*lda + j] - B[i*ldb + j];
        }
    }
}

// B <- A - B
void StrassenSingleProblem::matrix_subtract_inplace(int m, int n, float *A, int lda, float *B, int ldb) {
    for (int i = 0; i < n; i++) {
        cblas_saxpy(m, -1, A + i*lda, 1, B + i*ldb, 1);
    }
}

// Copy B into A
void StrassenSingleProblem::matrix_copy(int m, int n, float *A, int lda, float *B, int ldb) {
    for (int i = 0; i < n; i++) {
        memcpy(A + i*lda, B + i*ldb, m * sizeof(float));
    }
}
