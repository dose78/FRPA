#include "Task.h"
#include "Problem.h"
#define MAX_SIZE 100

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
