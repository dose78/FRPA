#include <mkl.h>
#include "Task.h"
#include "Problem.h"

class StrassenProblem: public Problem {
public:
    int n, m,k;
    double *A, *B, *C;
    StrassenProblem(int m, int k, int n, double *A, double *B, double *C);
    void runBaseCase();
    std::vector<Task*> split();
    void merge(std::vector<Problem*> subproblems);

private:
    void matrix_add(int N, double *A, double *B, double *C);
    void matrix_subtract(int N, double *A, double *B, double *C);
};
