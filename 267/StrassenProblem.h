#include <mkl.h>
#include "Task.h"
#include "Problem.h"
#define MAX_DEPTH 5

class StrassenProblem: public Problem {
    int n, m,k;
    double *A, *B, *C;

public:
    StrassenProblem(int m, int k, int n, double *A, double *B, double *C);
    bool shouldRunBaseCase(int depth);
    void runBaseCase();
    std::vector<Task*> split();
    void merge(std::vector<Problem*> subproblems);

private:
    void matrix_add(int N, double *A, double *B, double *C);
    void matrix_subtract(int N, double *A, double *B, double *C);
};
