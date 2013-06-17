#include <mkl.h>
#include "Task.h"
#include "Problem.h"
#define MAX_DEPTH 5

class SyrkProblem: public Problem {
public:
    int n, ldc, lda;
    double *C, *A;
    SyrkProblem(double *C, double *A, int n, int ldc, int lda);
    void runBaseCase();
    std::vector<Task*> split();
    std::vector<Problem*> splitSequential();
    void merge(std::vector<Problem*> subproblems);
    void mergeSequential(std::vector<Problem*> subproblems);
};
