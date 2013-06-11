#include <mkl.h>
#include "Task.h"
#include "Problem.h"
#define MAX_DEPTH 5

class TrsmProblem: public Problem {
public:
    int n, N;
    double *X, *T;
    TrsmProblem(double *X, double *T, int n, int N);
    bool shouldRunBaseCase(int depth);
    void runBaseCase();
    std::vector<Task*> split();
    void merge(std::vector<Problem*> subproblems);
};
