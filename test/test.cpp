#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <mkl.h>
using namespace std;
// #include <cilk/cilk.h>
// #include <cilk/cilk_api.h>

class TRSMProblem {
public:
    void printStuff() {
        printf("this is TRSM\n");
    }
};

class MultProblem {
public:
    void printStuff() {
        printf("this is Mult\n");
    }
};

class TRSMResult {
public:
    int x;
    void printStuff() {
        printf("this is TRSM Result. x = %d\n", x);
    }
};

class MultResult {
public:
    int y;
    void printStuff() {
        printf("this is Mult Result. y = %d\n", y);
    }
};

TRSMResult solveBase(TRSMProblem p) {
    // printf("getting TRSM subproblems\n");
    TRSMResult result;
    result.x = 19;
    return result;
}

MultResult solveBase(MultProblem p) {
    // printf("getting Mult subproblems\n");
    MultResult result;
    result.y = 17;
    return result;
}

template <class Problem, class Result>
Result solve (Problem problem, int depth = 5) {
    Result result = solveBase(problem);
    printf("depth: %d\n", depth);
    return result;
}

int main () {
    TRSMProblem trsmProb;
    MultProblem multProb;
    TRSMResult trsmResult = solve<TRSMProblem, TRSMResult>(trsmProb);
    MultResult multResult = solve<MultProblem, MultResult>(multProb, 1);
    trsmResult.printStuff();
    multResult.printStuff();
    return 0;
}
