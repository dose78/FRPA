#include <mkl.h>
#include "Task.h"
#include "Problem.h"

#define MAX_DEPTH 5

class MultProblem: public Problem {
public:
    int n, N;
    double *C, *A, *B;
    MultProblem(double *C, double *A, double *B, int n, int N);
    bool shouldRunBaseCase(int depth);
    void runBaseCase();
    std::vector<Task*> split();
    void merge(std::vector<Problem*> subproblems);
};
