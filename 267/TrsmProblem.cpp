#include "TrsmProblem.h"
#include "MultProblem.h"

TrsmProblem::TrsmProblem(int n, double *X, double *T) {
    this->n = n;
    this->T = T;
    this->X = X;
}

bool TrsmProblem::shouldRunBaseCase(int depth) {
    if (depth >= MAX_DEPTH || n <= 32) {
        return true;
    } else {
        return false;
    }
}

void TrsmProblem::runBaseCase() {
    // cblas_dtrsm(CblasColMajor, CblasRight, CblasLower, CblasTrans, CblasNonUnit, n, n, 1.0, T, n, X, n);
    printf("TRSM Base Case Ran\n");
}

std::vector<Problem*> TrsmProblem::split() {
    int n = 32;
    TrsmProblem* child1 = new TrsmProblem(n, X, T);
    MultProblem* child2 = new MultProblem(n, X, T);

    std::vector<Problem*> subproblems;
    subproblems.push_back(child1);
    subproblems.push_back(child2);
    return subproblems;
}
