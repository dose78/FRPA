#include "Task.h"
#include "Problem.h"

class MergesortProblem: public Problem {
private:
    double *A;
    int length;

public:
    MergesortProblem(double *A, int length);
    bool mustRunBaseCase();
    void runBaseCase();
    std::vector<Task*> split();
    void merge(std::vector<Problem*> subproblems);
};
