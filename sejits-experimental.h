#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <mkl.h>
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>

#define AS_MANY_AS_POSSIBLE -1
#define ONLY_ONE 1

typedef struct {
  Problem* problems;
  int count;
  int contains_last_subproblem;
} Subproblems;

// Methods to be implemented
int should_run_base_case(Problem problem, int depth);
Result base_case(Problem problem);
Subproblems get_subproblems(Problem problem, Result final_result, int num_solved, int num_to_generate);
Result merge(Resutl final_result, Result* results, Subproblems subproblems);

Result solve(Problem problem, Result prev_result, int depth) {
  if (should_run_base_case(problem, depth)) {
    return base_case(prev_results, num_prev_results, problem);
  }

  Result final_result = NULL;
  Subproblems subproblems;
  int i, num_solved = 0;

  int BFS = depth % 2;
  if (BFS) {
    do {
      subproblems = get_subproblems(problem, final_result, num_solved, AS_MANY_AS_POSSIBLE);
      Result* results = malloc(subproblems.count * sizeof(Result));
      for (i = 0; i < subproblems.count; i++) {
        results[i] = cilk_spawn solve(subproblems.problems[i], final_result, depth + 1);
      }
      cilk_sync;
      merge(final_result, results, subproblems);
      free(results);
      num_solved += subproblems.count;
    } while (subproblems.contains_last_subproblem != 1)

  } else { // DFS
    do {
      subproblems = get_subproblems(problem, final_result, num_solved, ONLY_ONE);
      Result result = solve(subproblems.problems[0], final_result, depth + 1);
      merge(final_result, &result, subproblems);
      num_solved += 1;
    } while (subproblems.contains_last_subproblem != 1)
  }

  return final_result;
}