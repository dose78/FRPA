#include "Task.h"
#include "Problem.h"

class QuicksortProblem: public Problem {
private:
    double *A;
    int length;

public:
    QuicksortProblem(double *A, int length);
    bool mustRunBaseCase();
    void runBaseCase();
    std::vector<Task*> split();
    void merge(std::vector<Problem*> subproblems);
};
