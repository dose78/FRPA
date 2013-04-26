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
    TrsmProblem* problem = new TrsmProblem(n, X, T);

    // if (problem->shouldRunBaseCase(1)) {
    //     printf("true\n");
    //     problem->runBaseCase();
    // } else {
    //     printf("false\n");
    // }

    std::vector<Problem*> subproblems = problem->split();

    for(std::vector<Problem*>::iterator it = subproblems.begin(); it != subproblems.end(); it++) {
        Problem *p = *it;
        p->runBaseCase();
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
