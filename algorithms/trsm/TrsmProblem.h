#include <mkl.h>
#include "Task.h"
#include "Problem.h"

class TrsmProblem: public Problem {
public:
    int n, N;
    double *X, *T;
    TrsmProblem(double *X, double *T, int n, int N);
    void runBaseCase();
    std::vector<Task*> split();
    void merge(std::vector<Problem*> subproblems);
};
