#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <mkl.h>
// #include <cilk/cilk.h>
// #include <cilk/cilk_api.h>

#define DONE_SOLVING -1
#define AS_MANY_AS_POSSIBLE -1
#define ONLY_ONE 1

template <class Problem>
void solve(Problem problem, int depth) {
    if (problem.should_run_base_case(depth)) {
        problem.run_base_case();
        return;
    }

    int i, num_solved, num_allocated, next_to_merge = 0;
    int BFS = depth % 2;
    if (BFS) {
        do {
            num_allocated = problem.allocate_subproblems(num_solved, AS_MANY_AS_POSSIBLE);
            for (i = 0; i < num_allocated; i++) {
                // cilk_spawn problem.solve_subproblem(i, depth + 1);
                problem.solve_subproblem(num_solved++, depth + 1);
            }
            // cilk_sync;
            next_to_merge = problem.merge(num_solved, next_to_merge);
        } while (next_to_merge != DONE_SOLVING)

    } else { // DFS
        do {
            problem.allocate_subproblems(num_solved, ONLY_ONE);
            problem.solve_subproblem(num_solved++, depth + 1);
            next_to_merge = problem.merge(num_solved, next_to_merge);
        } while (next_to_merge != DONE_SOLVING)
    }
    return final_result;
}
