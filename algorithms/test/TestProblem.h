#include <mkl.h>
#include "Task.h"
#include "Problem.h"

class TestProblem: public Problem {
public:
    int n;
    double *A;
    TestProblem(double *A, int n);
    void runBaseCase();
    std::vector<Task*> split();
    void merge(std::vector<Problem*> subproblems);
};
