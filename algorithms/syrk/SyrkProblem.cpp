#include "MultProblem.h"
#include "SyrkProblem.h"
#include "memorytracking.h"

SyrkProblem::SyrkProblem(double *C, double *A, int n, int ldc, int lda) {
    this->C = C; this->A = A;
    this->ldc = ldc; this->lda = lda;
    this->n = n;
}

void SyrkProblem::runBaseCase() {
    cblas_dsyrk(CblasColMajor, CblasLower, CblasNoTrans, n, n, -1.0, A, lda, 1.0, C, ldc);
}

std::vector<Task*> SyrkProblem::split() {
    double* C11 = C;
    double* C12 = C + ldc*n/2;
    double* C21 = C + n/2;
    double* C22 = C + ldc*n/2 + n/2;

    double* A11 = A;
    double* A12 = A + lda*n/2;
    double* A21 = A + n/2;
    double* A22 = A + lda*n/2 + n/2;

    double *C11_2 = (double*) malloc(n * n / 4 * sizeof(double));
    double *C21_2 = (double*) malloc(n * n / 4 * sizeof(double));
    double *C22_2 = (double*) malloc(n * n / 4 * sizeof(double));
    memset(C11_2, 0, n * n / 4 * sizeof(double));
    memset(C21_2, 0, n * n / 4 * sizeof(double));
    memset(C22_2, 0, n * n / 4 * sizeof(double));

    std::vector<Task*> tasks (6);
    tasks[0] = new Task(new SyrkProblem(C11, A11, n/2, ldc, lda));
    tasks[1] = new Task(new SyrkProblem(C11_2, A12, n/2, n/2, lda));
    tasks[2] = new Task(new MultProblem(C21, A21, A11, n/2, ldc, lda, lda));
    tasks[3] = new Task(new MultProblem(C21_2, A22, A12, n/2, n/2, lda, lda));
    tasks[4] = new Task(new SyrkProblem(C22, A21, n/2, ldc, lda));
    tasks[5] = new Task(new SyrkProblem(C22_2, A22, n/2, n/2, lda));
    return tasks;
}

void SyrkProblem::merge(std::vector<Problem*> subproblems) {
    double* C11 = C;
    double* C12 = C + ldc*n/2;
    double* C21 = C + n/2;
    double* C22 = C + ldc*n/2 + n/2;

    SyrkProblem* syrk1 = static_cast<SyrkProblem*>(subproblems[1]);
    double* C11_2 = syrk1->C;
    for (int x = 0; x < n/2; x++) {
        cblas_daxpy(n/2, 1, C11_2 + n/2 * x, 1, C11 + ldc * x, 1);
    }
    free(C11_2);

    MultProblem* mult1 = static_cast<MultProblem*>(subproblems[3]);
    double* C21_2 = mult1->C;
    for (int x = 0; x < n/2; x++) {
        cblas_daxpy(n/2, 1, C21_2 + n/2 * x, 1, C21 + ldc * x, 1);
    }
    free(C21_2);

    SyrkProblem* syrk2 = static_cast<SyrkProblem*>(subproblems[5]);
    double* C22_2 = syrk2->C;
    for (int x = 0; x < n/2; x++) {
        cblas_daxpy(n/2, 1, C22_2 + n/2 * x, 1, C22 + ldc * x, 1);
    }
    free(C22_2);
}

std::vector<Problem*> SyrkProblem::splitSequential() {
    double* C11 = C;
    double* C12 = C + ldc*n/2;
    double* C21 = C + n/2;
    double* C22 = C + ldc*n/2 + n/2;

    double* A11 = A;
    double* A12 = A + lda*n/2;
    double* A21 = A + n/2;
    double* A22 = A + lda*n/2 + n/2;

    std::vector<Problem*> subproblems (6);
    subproblems[0] = new SyrkProblem(C11, A11, n/2, ldc, lda);
    subproblems[1] = new SyrkProblem(C11, A12, n/2, ldc, lda);
    subproblems[2] = new MultProblem(C21, A21, A11, n/2, ldc, lda, lda);
    subproblems[3] = new MultProblem(C21, A22, A12, n/2, ldc, lda, lda);
    subproblems[4] = new SyrkProblem(C22, A21, n/2, ldc, lda);
    subproblems[5] = new SyrkProblem(C22, A22, n/2, ldc, lda);
    return subproblems;
}

void SyrkProblem::mergeSequential(std::vector<Problem*> subproblems) {
    return;
}
