#include <mkl.h>
#include "Task.h"
#include "Problem.h"
#define MAX_DEPTH 5

class SyrkProblem: public Problem {
public:
    int n, N;
    double *C, *A;
    SyrkProblem(double *C, double *A, int n, int N);
    bool shouldRunBaseCase(int depth);
    void runBaseCase();
    std::vector<Task*> split();
    void merge(std::vector<Problem*> subproblems);
};
