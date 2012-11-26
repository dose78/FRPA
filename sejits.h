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

// Methods to be implemented
int should_run_base_case(Problem problem, int depth);
Result base_case(Problem p);
int get_num_subproblems(Problem p);
Problem get_next_subproblem(Problem p, Problem* subproblems, int prob_num);
Result merge(Result* results);

Result solve(Problem problem, int depth) {
  if (should_run_base_case(problem, depth)) {
    return base_case(problem);
  }

  int num_subproblems = get_num_subproblems(problem);
  Problem* subproblems = NULL;

  int i;
  for (i = 0; i < num_subproblems; i++) {
    Problem* new_subproblems = realloc(subproblems, (i+1) * sizeof(Problem));
    if (new_subproblems) {
      subproblems = new_subproblems;
      subproblems[i] = get_next_subproblem(problem, subproblems, i);
    } else {
      // memory could not be allocated
    }
  }

  Result* results = malloc(num_subproblems * sizeof(Result));

  for (i = 0; i < num_subproblems; i++) {
    results[i] = cilk_spawn solve(subproblems[i], depth + 1);
  }
  cilk_sync;

  free(subproblems);
  Result result = merge(results);
  free(results);
  return result;
}
