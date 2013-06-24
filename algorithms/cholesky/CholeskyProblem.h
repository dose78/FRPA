#include <mkl.h>
#include "Task.h"
#include "Problem.h"

class CholeskyProblem: public Problem {
public:
    int n, N;
    double *A;
    CholeskyProblem(double *A, int n, int N);
    void runBaseCase();
    std::vector<Task*> split();
    void merge(std::vector<Problem*> subproblems);
};
