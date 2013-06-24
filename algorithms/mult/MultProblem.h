#include <mkl.h>
#include "Task.h"
#include "Problem.h"

class MultProblem: public Problem {
public:
    int n, ldc, lda, ldb;
    double *C, *A, *B;
    MultProblem(double *C, double *A, double *B, int n, int ldc, int lda, int ldb);
    bool mustRunBaseCase();
    void runBaseCase();
    std::vector<Task*> split();
    void merge(std::vector<Problem*> subproblems);
};
