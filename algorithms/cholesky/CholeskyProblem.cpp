#include "TrsmProblem.h"
#include "SyrkProblem.h"
#include "CholeskyProblem.h"
#include "memorytracking.h"

CholeskyProblem::CholeskyProblem(double *A, int n, int N) {
    this->A = A;
    this->n = n;
    this->N = N;
}

void CholeskyProblem::runBaseCase() {
    int info;
    char *Lc = "L";
    dpotrf(Lc, &n, A, &N, &info);
}

std::vector<Task*> CholeskyProblem::split() {
    double* A11 = A;
    double* A12 = A + N*n/2;
    double* A21 = A + n/2;
    double* A22 = A + N*n/2 + n/2;

    std::vector<Task*> tasks (1);
    tasks[0] = new Task(4);
    tasks[0]->addProblem(new CholeskyProblem(A11, n/2, N));
    tasks[0]->addProblem(new TrsmProblem(A21, A11, n/2, N));
    tasks[0]->addProblem(new SyrkProblem(A22, A21, n/2, N, N));
    tasks[0]->addProblem(new CholeskyProblem(A22, n/2, N));
    return tasks;
}

void CholeskyProblem::merge(std::vector<Problem*> subproblems) {
    return;
}
