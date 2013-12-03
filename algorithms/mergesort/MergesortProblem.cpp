#include "MergesortProblem.h"
#include "memorytracking.h"

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

bool MergesortProblem::mustRunBaseCase() {
    return (length <= 1);
}

void MergesortProblem::runBaseCase() {
    qsort(A, length, sizeof(double), cmp);
}

std::vector<Task*> MergesortProblem::split() {
    int midpoint = length/2;
    std::vector<Task*> tasks (2);
    tasks[0] = new Task(new MergesortProblem(A, midpoint));
    tasks[1] = new Task(new MergesortProblem(A + midpoint, midpoint + (length % 2)));
    return tasks;
}

void MergesortProblem::merge(std::vector<Problem*> subproblems) {
    MergesortProblem* s1 = (MergesortProblem*) subproblems[0];
    MergesortProblem* s2 = (MergesortProblem*) subproblems[1];

    double *merged = (double*) malloc(length * sizeof(double));
    int p1 = 0, p2 = 0, i = 0;

    // merge
    while (p1 != s1->length && p2 != s2->length) {
        if (s1->A[p1] < s2->A[p2]) {
            merged[i] = s1->A[p1];
            p1++;
        } else {
            merged[i] = s2->A[p2];
            p2++;
        }
        i++;
    }

    // move leftovers
    if (p2 == s2->length) {
        memcpy(A+i, s1->A+p1, (length-i) * sizeof(double));
    }

    // copy merged array into A
    memcpy(A, merged, i * sizeof(double));

    free(merged);
}
