#include "QuicksortProblem.h"
#include "memorytracking.h"

int cmp(const void *x, const void *y) {
    double xx = *(double*)x, yy = *(double*)y;
    if (xx < yy) return -1;
    if (xx > yy) return  1;
    return 0;
}

QuicksortProblem::QuicksortProblem(double *A, int length) {
    this->A = A;
    this->length = length;
}

bool QuicksortProblem::mustRunBaseCase() {
    return (length <= 1);
}

void QuicksortProblem::runBaseCase() {
    qsort(A, length, sizeof(double), cmp);
}

std::vector<Task*> QuicksortProblem::split() {
    int i = 0, j = length - 2;
    double pivot = A[length - 1], temp;
    do {
        while (A[i] < pivot) i++;
        while (A[j] > pivot && j > 0) j--;
        if (i < j) {
            temp = A[i];
            A[i] = A[j];
            A[j] = temp;
        }
    } while (i < j);
    A[length - 1] = A[i];
    A[i] = pivot;

    std::vector<Task*> tasks (2);
    tasks[0] = new Task(new QuicksortProblem(A, i));
    tasks[1] = new Task(new QuicksortProblem(A + i + 1, length - i - 1));
    return tasks;
}

void QuicksortProblem::merge(std::vector<Problem*> subproblems) {
    return;
}
