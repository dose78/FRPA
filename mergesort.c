#include "mergesort.h"
#include "sejits.h"

int get_num_subproblems(Problem p) {
  return 2;
}

Problem get_next_subproblem(Problem p, Problem* subproblems, int prob_num) {
  if (prob_num == 0) {
    int midpoint = p.length/2;
    Problem subproblem1 = {p.A, midpoint};
    return subproblem1;
  } else {
    int midpoint = subproblems[0].length;
    Problem subproblem2 = {p.A + midpoint, midpoint + (p.length % 2)};
    return subproblem2;
  }
}

Result merge(Result* results) {
  Result r1 = results[0];
  Result r2 = results[1];
  int length = r1.length + r2.length;
  double *merged = malloc(length* sizeof(double));
  int p1 = 0, p2 = 0, i = 0;
  while (i < length) {
    if ( (p1 != r1.length && r1.A[p1] <= r2.A[p2]) || (p2 == r2.length) ) {
      merged[i] = r1.A[p1];
      p1++;
    } else {
      merged[i] = r2.A[p2];
      p1++;
    }
    i++;
  }

  for (i = 0; i < length; i++) {
    r1.A[i] = merged[i];
  }
  free(merged);

  Result r = {r1.A, length};
  return r;
}

int should_run_base_case(Problem problem, int depth) {
  if (problem.length < 2) {
    return 1;
  } else {
    return 0;
  }
}

Result base_case(Problem p) {
  Result r = {p.A, p.length};
  return r;
}

void sort(double *A, int length) {
  Problem problem = {A, length};
  solve(problem, 0);
}
