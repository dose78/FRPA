#include "CarmaSingleProblem.h"
#include "memorytracking.h"

int CarmaSingleProblem::getDimToSplit() {
    if (n >= k && n >= m) return SPLIT_N;
    if (m >= k && m >= n) return SPLIT_M;
    return SPLIT_K;
}

CarmaSingleProblem::CarmaSingleProblem(int M, int K, int m, int k, int n, int CM, float *A, float *B, float *C) {
    this->M = M; this->K = K; this->CM = CM;
    this->m = m; this->k = k; this->n = n;
    this->A = A; this->B = B; this->C = C;
}

void CarmaSingleProblem::runBaseCase() {
    cblas_sgemm(CblasColMajor,CblasNoTrans,CblasNoTrans,m,n,k,1,A,M,B,K,1,C,CM);
}

std::vector<Task*> CarmaSingleProblem::split() {
    Task* task1 = new Task(1);
    Task* task2 = new Task(1);
    int dim = getDimToSplit();
    if (dim == SPLIT_N) {
        float *B1 = B, *B2 = B + n/2*K;
        task1->addProblem(new CarmaSingleProblem(M, K, m, k, n/2, CM, A, B1, C));
        task2->addProblem(new CarmaSingleProblem(M, K, m, k, n/2, CM, A, B2, C + n/2*CM));
    } else if (dim == SPLIT_M) {
        float *A1 = A, *A2 = A + m/2;
        task1->addProblem(new CarmaSingleProblem(M, K, m/2, k, n, CM, A1, B, C));
        task2->addProblem(new CarmaSingleProblem(M, K, m/2, k, n, CM, A2, B, C + m/2));
    } else { // SPLIT_K
        float *A1 = A, *A2 = A + k/2*M;
        float *B1 = B, *B2 = B + k/2;
        float *Q1 = (float*) malloc(m * n * sizeof(float));
        memset(Q1, 0, sizeof(float) * m * n);
        task1->addProblem(new CarmaSingleProblem(M, K, m, k/2, n, m, A1, B1, Q1));
        task2->addProblem(new CarmaSingleProblem(M, K, m, k/2, n, CM, A2, B2, C));
    }
    std::vector<Task*> tasks (2);
    tasks[0] = task1;
    tasks[1] = task2;
    return tasks;
}

std::vector<Problem*> CarmaSingleProblem::splitSequential() {
    std::vector<Problem*> subproblems (2);
    int dim = getDimToSplit();
    if (dim == SPLIT_N) {
        float *B1 = B, *B2 = B + n/2*K;
        subproblems[0] = new CarmaSingleProblem(M, K, m, k, n/2, CM, A, B1, C);
        subproblems[1] = new CarmaSingleProblem(M, K, m, k, n/2, CM, A, B2, C + n/2*CM);
    } else if (dim == SPLIT_M) {
        float *A1 = A, *A2 = A + m/2;
        subproblems[0] = new CarmaSingleProblem(M, K, m/2, k, n, CM, A1, B, C);
        subproblems[1] = new CarmaSingleProblem(M, K, m/2, k, n, CM, A2, B, C + m/2);
    } else { // SPLIT_K
        float *A1 = A, *A2 = A + k/2*M;
        float *B1 = B, *B2 = B + k/2;
        subproblems[0] = new CarmaSingleProblem(M, K, m, k/2, n, CM, A1, B1, C);
        subproblems[1] = new CarmaSingleProblem(M, K, m, k/2, n, CM, A2, B2, C);
    }
    return subproblems;
}

void CarmaSingleProblem::merge(std::vector<Problem*> subproblems) {
    int dim = getDimToSplit();
    if (dim == SPLIT_K) {
        CarmaSingleProblem* subproblem = static_cast<CarmaSingleProblem*>(subproblems[0]);
        for (int x = 0; x < subproblem->n; x++) {
            cblas_saxpy(subproblem->m, 1, subproblem->C + subproblem->m * x, 1, C + CM * x, 1);
        }
        free(subproblem->C);
    }
}

void CarmaSingleProblem::mergeSequential(std::vector<Problem*> subproblems) {
    return;
}
