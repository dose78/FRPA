#include <mkl.h>
#include "Task.h"
#include "Problem.h"

#define SPLIT_M 1
#define SPLIT_K 2
#define SPLIT_N 3

class CarmaProblem: public Problem {
public:
    int M, K, m, k, n, CM;
    double *A, *B, *C;
    CarmaProblem(int M, int K, int m, int k, int n, int CM, double *A, double *B, double *C);
    void runBaseCase();
    std::vector<Task*> split();
    std::vector<Problem*> splitSequential();
    void merge(std::vector<Problem*> subproblems);
    void mergeSequential(std::vector<Problem*> subproblems);
    int getDimToSplit();
};
