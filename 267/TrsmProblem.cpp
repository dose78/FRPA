#include "TrsmProblem.h"
#include "MultProblem.h"

TrsmProblem::TrsmProblem(double *X, double *T, int n, int N) {
    this->X = X;
    this->T = T;
    this->n = n;
    this->N = N;
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

std::vector<Task*> TrsmProblem::split() {
    double* X11 = X;
    double* X12 = X + N*n/2;
    double* X21 = X + n/2;
    double* X22 = X + N*n/2 + n/2;

    double* T11 = T;
    double* T12 = T + N*n/2;
    double* T21 = T + n/2;
    double* T22 = T + N*n/2 + n/2;

    Task* task1 = new Task();
    task1->addProblem(new TrsmProblem(X11, T11, n/2, N));
    task1->addProblem(new MultProblem(X12, X11, T21, n/2, N));
    task1->addProblem(new TrsmProblem(X12, T22, n/2, N));

    Task* task2 = new Task();
    task2->addProblem(new TrsmProblem(X21, T11, n/2, N));
    task2->addProblem(new MultProblem(X22, X21, T21, n/2, N));
    task2->addProblem(new TrsmProblem(X22, T22, n/2, N));

    std::vector<Task*> tasks;
    tasks.push_back(task1);
    tasks.push_back(task2);
    return tasks;
}

void TrsmProblem::merge(std::vector<Problem*>) {
    printf("TRSM Merge\n");
}

