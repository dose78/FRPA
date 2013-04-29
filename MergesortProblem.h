#include "Task.h"
#include "Problem.h"
#define MAX_DEPTH 5

class MergesortProblem: public Problem {
private:
    double *A;
    int length;

public:
    MergesortProblem(double *A, int length);
    bool shouldRunBaseCase(int depth);
    void runBaseCase();
    std::vector<Task*> split();
    void merge(std::vector<Problem*> subproblems);
};
