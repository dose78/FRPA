#include "MergesortProblem.h"

MergesortProblem::MergesortProblem(double *A, int length) {
    this->A = A;
    this->length = length;
}

bool MergesortProblem::shouldRunBaseCase(int depth) {
    return (length < 2);
}

void MergesortProblem::runBaseCase() {
    return;
}

std::vector<Task*> MergesortProblem::split() {
    int midpoint = length/2;
    Task* task1 = new Task(new MergesortProblem(A, midpoint));
    Task* task2 = new Task(new MergesortProblem(A + midpoint, midpoint + (length % 2)));

    std::vector<Task*> tasks (2);
    tasks[0] = task1;
    tasks[1] = task2;
    return tasks;
}

void MergesortProblem::merge(std::vector<Problem*> subproblems) {
    MergesortProblem* r1 = static_cast<MergesortProblem*>(subproblems[0]);
    MergesortProblem* r2 = static_cast<MergesortProblem*>(subproblems[1]);

    int length = r1->length + r2->length;
    double *r1A = r1->A;
    double *r2A = r2->A;
    double *merged = (double*) malloc(length * sizeof(double));
    int p1 = 0, p2 = 0, i = 0;
    while (i < length) {
        if ( (p1 != r1->length && r1A[p1] <= r2A[p2]) || (p2 == r2->length) ) {
            merged[i] = r1A[p1];
            p1++;
        } else {
            merged[i] = r2A[p2];
            p2++;
        }
        i++;
    }

    memcpy(A, merged, length*sizeof(double));
    free(merged);
}
