#include "Task.h"
#include "Problem.h"
#define MAX_SIZE 100

class QuicksortProblem: public Problem {
public:
    double *A;
    int length;
    QuicksortProblem(double *A, int length);
    bool mustRunBaseCase();
    void runBaseCase();
    std::vector<Task*> split();
    void merge(std::vector<Problem*> subproblems);
};
