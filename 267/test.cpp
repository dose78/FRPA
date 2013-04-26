#include "TrsmProblem.h"
#include "MultProblem.h"


void initialize(int n, double *X, double *T) {
    srand48(time(NULL));
    int i;
    for(i = 0; i < n*n; i++) { X[i] = 2 * drand48() - 1;}
    for(i = 0; i < n*n; i++) { T[i] = 2 * drand48() - 1;}
}


int main() {
    int n = 64;
    double *X = (double*) malloc(n * n * sizeof(double));
    double *T = (double*) malloc(n * n * sizeof(double));
    TrsmProblem* problem = new TrsmProblem(X, T, n, n);

    // if (problem->shouldRunBaseCase(1)) {
    //     printf("true\n");
    //     problem->runBaseCase();
    // } else {
    //     printf("false\n");
    // }

    std::vector<Task*> tasks = problem->split();

    for(std::vector<Task*>::iterator it = tasks.begin(); it != tasks.end(); it++) {
        Task *t = *it;
        std::vector<Problem*> problems = t->getProblems();
        for(std::vector<Problem*>::iterator it2 = problems.begin(); it2 != problems.end(); it2++) {
            Problem *p = *it2;
            p->runBaseCase();
        }
    }

    // for(std::vector<Problem*>::iterator it = subproblems.begin(); it != subproblems.end(); it++) {
    //     Problem *p = *it;
    //     free(p->X);
    //     free(p->T);
    //     delete p;
    // }

    free(X);
    free(T);
    delete problem;
}
