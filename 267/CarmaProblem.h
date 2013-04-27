#include "Task.h"
#include "Problem.h"
#define SPLIT_M 1
#define SPLIT_K 2
#define SPLIT_N 3
#define MAX_DEPTH 5

class CarmaProblem: public Problem {
    int M, K, m, k, n, CM;
    double *A, *B, *C;

public:
    CarmaProblem(int M, int K, int m, int k, int n, int CM, double *A, double *B, double *C);
    bool shouldRunBaseCase(int depth);
    void runBaseCase();
    std::vector<Task*> split();
    void merge(std::vector<Problem*> subproblems);
    int getDimToSplit();
};
