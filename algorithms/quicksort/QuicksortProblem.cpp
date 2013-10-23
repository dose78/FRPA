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
    return (length < MAX_SIZE);
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

    Task* task1 = new Task(new QuicksortProblem(A, i));
    Task* task2 = new Task(new QuicksortProblem(A + i + 1, length - i - 1));

    std::vector<Task*> tasks (2);
    tasks[0] = task1;
    tasks[1] = task2;
    return tasks;
}

void QuicksortProblem::merge(std::vector<Problem*> subproblems) {
    return;
}
