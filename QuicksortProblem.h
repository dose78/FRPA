#include "Task.h"
#include "Problem.h"

class QuicksortProblem: public Problem {
public:
    double *A;
    int length;
    QuicksortProblem(double *A, int length);
    bool shouldRunBaseCase(int depth);
    void runBaseCase();
    std::vector<Task*> split();
    void merge(std::vector<Problem*> subproblems);
};
