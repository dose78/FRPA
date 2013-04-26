// #include "Task.h"
#include "Problem.h"
#define MAX_DEPTH 5

class TrsmProblem: public Problem {
    int n;
    double *T, *X;

public:
    TrsmProblem(int n, double *X, double *T);
    bool shouldRunBaseCase(int depth);
    void runBaseCase();
    std::vector<Problem*> split();
};
