#include "MultProblem.h"

MultProblem::MultProblem(double *C, double *A, double *B, int n, int N) {
    this->C = C;
    this->A = A;
    this->B = B;
    this->n = n;
    this->N = N;
}

bool MultProblem::shouldRunBaseCase(int depth) {
    return true;
}

void MultProblem::runBaseCase() {
    // cblas_dtrsm(CblasColMajor, CblasRight, CblasLower, CblasTrans, CblasNonUnit, n, n, 1.0, T, n, X, n);
    printf("MULT %d Base Case Ran\n",n);
}

std::vector<Task*> MultProblem::split() {
    std::vector<Task*> tasks;
    return tasks;
}

void MultProblem::merge(std::vector<Problem*>) {
    printf("MULT Merge\n");
}
