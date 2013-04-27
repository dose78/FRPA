#include "Task.h"
#include "Problem.h"

class MergesortProblem: public Problem {
    double *A;
    int length;

public:
    MergesortProblem(double *A, int length);
    bool shouldRunBaseCase(int depth);
    void runBaseCase();
    std::vector<Task*> split();
    void merge(std::vector<Problem*> subproblems);
};
