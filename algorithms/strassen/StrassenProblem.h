#include <mkl.h>
#include "Task.h"
#include "Problem.h"

class StrassenProblem: public Problem {
public:
    int n, m, k, lda, ldb, ldc;
    double *A, *B, *C;
    StrassenProblem(int m, int k, int n, double *A, int lda, double *B, int ldb, double *C, int ldc);
    void runBaseCase();
    std::vector<Task*> split();
    void merge(std::vector<Problem*> subproblems);

private:
    void matrix_add(int m, int n, double *A, int lda, double *B, int ldb, double *C, int ldc);
    void matrix_add_inplace(int m, int n, double *A, int lda, double *B, int ldb);
    void matrix_subtract(int m, int n, double *A, int lda, double *B, int ldb, double *C, int ldc);
    void matrix_subtract_inplace(int m, int n, double *A, int lda, double *B, int ldb);
    void matrix_copy(int m, int n, double *A, int lda, double *B, int ldb);
};
