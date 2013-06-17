#include "Task.h"
#include "Problem.h"
#define MAX_DEPTH 5

class MergesortProblem: public Problem {
public:
    double *A;
    int length;
    MergesortProblem(double *A, int length);
    void runBaseCase();
    std::vector<Task*> split();
    void merge(std::vector<Problem*> subproblems);
};
