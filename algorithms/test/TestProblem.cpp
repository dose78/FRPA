#include "TestProblem.h"
#include "memorytracking.h"

TestProblem::TestProblem(double *A, int n) {
    this->A = A;
    this->n = n;
}

void TestProblem::runBaseCase() {
    return;
}

std::vector<Task*> TestProblem::split() {
    std::vector<Task*> tasks (7);
    for (int i = 0; i < 7; i++) {
        double *A2 = (double*) malloc(n * sizeof(double));
        // for(int i = 0; i < n; i++) A2[i] = 2 * drand48() - 1;
        tasks[i] = new Task(new TestProblem(A2, n));
    }
    return tasks;
}

void TestProblem::merge(std::vector<Problem*> subproblems) {
    for(std::vector<Problem*>::iterator problemIterator = subproblems.begin(); problemIterator != subproblems.end(); problemIterator++) {
        TestProblem *problem = (TestProblem*)*problemIterator;
        free((double*) problem->A);
    }
}
