#include <mkl.h>
#include "Task.h"
#include "Problem.h"

class StrassenSingleNaiveProblem: public Problem {
public:
    int n, m, k;
    float *A, *B, *C;
    StrassenSingleNaiveProblem(int m, int k, int n, float *A, float *B, float *C);
    void runBaseCase();
    std::vector<Task*> split();
    void merge(std::vector<Problem*> subproblems);

private:
    void matrix_add(int m, int n, float *A, int lda, float *B, int ldb, float *C, int ldc);
    void matrix_subtract(int m, int n, float *A, int lda, float *B, int ldb, float *C, int ldc);
    void matrix_copy(int m, int n, float *A, int lda, float *B, int ldb);
};
