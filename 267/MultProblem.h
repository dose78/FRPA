// #include "Task.h"
#include "Problem.h"
#define MAX_DEPTH 5

class MultProblem: public Problem {
    int n;
    double *A, *B;

public:
    MultProblem(int n, double *A, double *B);
    bool shouldRunBaseCase(int depth);
    void runBaseCase();
    std::vector<Problem*> split();
};
