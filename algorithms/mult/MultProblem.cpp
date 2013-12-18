#include "MultProblem.h"
#include "memorytracking.h"

MultProblem::MultProblem(double *C, double *A, double *B, int n, int ldc, int lda, int ldb) {
    this->C = C; this->A = A; this->B = B;
    this->ldc = ldc; this->lda = lda; this->ldb = ldb;
    this->n = n;
}

bool MultProblem::mustRunBaseCase() {
    return true;
}

void MultProblem::runBaseCase() {
    cblas_dgemm(CblasColMajor,CblasNoTrans,CblasTrans,n,n,n,-1.0,A,lda,B,ldb,1.0,C,ldc);
}

std::vector<Task*> MultProblem::split() {
    std::vector<Task*> tasks;
    return tasks;
}

void MultProblem::merge(std::vector<Problem*> subproblems) { }
