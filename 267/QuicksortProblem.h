#include "Task.h"
#include "Problem.h"

class QuicksortProblem: public Problem {
    double *A;
    int length;

public:
    QuicksortProblem(double *A, int length);
    bool shouldRunBaseCase(int depth);
    void runBaseCase();
    std::vector<Task*> split();
    void merge(std::vector<Problem*> subproblems);
};
