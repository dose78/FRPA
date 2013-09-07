#include <mkl.h>
#include "Task.h"
#include "Problem.h"

class StrassenSingleProblem: public Problem {
public:
    int n, m, k, lda, ldb, ldc;
    float *A, *B, *C;
    StrassenSingleProblem(int m, int k, int n, float *A, int lda, float *B, int ldb, float *C, int ldc);
    void runBaseCase();
    std::vector<Task*> split();
    void merge(std::vector<Problem*> subproblems);

private:
    void matrix_add(int m, int n, float *A, int lda, float *B, int ldb, float *C, int ldc);
    void matrix_add_inplace(int m, int n, float *A, int lda, float *B, int ldb);
    void matrix_subtract(int m, int n, float *A, int lda, float *B, int ldb, float *C, int ldc);
    void matrix_subtract_inplace(int m, int n, float *A, int lda, float *B, int ldb);
    void matrix_copy(int m, int n, float *A, int lda, float *B, int ldb);
};
