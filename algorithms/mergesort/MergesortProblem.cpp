#include "MergesortProblem.h"

int cmp(const void *x, const void *y) {
    double xx = *(double*)x, yy = *(double*)y;
    if (xx < yy) return -1;
    if (xx > yy) return  1;
    return 0;
}

MergesortProblem::MergesortProblem(double *A, int length) {
    this->A = A;
    this->length = length;
}

void MergesortProblem::runBaseCase() {
    qsort(A, length, sizeof(double), cmp);
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

    int r1Length = r1->length;
    int r2Length = r2->length;
    double *r1A = r1->A;
    double *r2A = r2->A;
    double *merged = (double*) malloc(length * sizeof(double));
    int p1 = 0, p2 = 0, i = 0;

    // merge
    while (p1 != r1Length && p2 != r2Length) {
        if (r1A[p1] < r2A[p2]) {
            merged[i] = r1A[p1];
            p1++;
        } else {
            merged[i] = r2A[p2];
            p2++;
        }
        i++;
    }

    // move leftovers if  p1 did not finish
    if (p2 == r2Length) {
        memcpy(A+i, r1A+p1, (length-i) * sizeof(double));
    }

    memcpy(A, merged, i * sizeof(double));

    free(merged);
}
