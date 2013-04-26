#include "MultProblem.h"

MultProblem::MultProblem(int n, double *A, double *B) {
    this->n = n;
    this->A = A;
    this->B = B;
}

bool MultProblem::shouldRunBaseCase(int depth) {
    if (depth >= MAX_DEPTH || n <= 32) {
        return true;
    } else {
        return false;
    }
}

void MultProblem::runBaseCase() {
    // cblas_dtrsm(CblasColMajor, CblasRight, CblasLower, CblasTrans, CblasNonUnit, n, n, 1.0, T, n, X, n);
    printf("MULT Base Case Ran\n");
}

std::vector<Problem*> MultProblem::split() {
    int n = 32;
    MultProblem* child1 = new MultProblem(n, A, B);
    MultProblem* child2 = new MultProblem(n, A, B);

    std::vector<Problem*> subproblems;
    subproblems.push_back(child1);
    subproblems.push_back(child2);
    return subproblems;
}
