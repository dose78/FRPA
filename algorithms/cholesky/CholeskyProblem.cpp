#include "CholeskyProblem.h"
#include "TrsmProblem.h"
#include "SyrkProblem.h"

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

    Task* task1 = new Task(4);
    task1->addProblem(new CholeskyProblem(A11, n/2, N));
    task1->addProblem(new TrsmProblem(A21, A11, n/2, N));
    task1->addProblem(new SyrkProblem(A22, A21, n/2, N, N));
    task1->addProblem(new CholeskyProblem(A22, n/2, N));

    std::vector<Task*> tasks (1);
    tasks[0] = task1;
    return tasks;
}

void CholeskyProblem::merge(std::vector<Problem*> subproblems) {
    return;
}

