#include "Problem.h"

void solve(Problem problem, int depth) {
    if (problem.shouldRunBaseCase()) {
        problem.runBaseCase();
        return;
    }
    int BFS = depth % 2;

    if (BFS) {

    }
}
