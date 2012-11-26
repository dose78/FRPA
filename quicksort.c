#include "quicksort.h"
#include "sejits.h"

int get_num_subproblems(Problem p) {
  return 2;
}

Problem get_next_subproblem(Problem p, Problem* subproblems, int prob_num) {
  if (prob_num == 0) {
    int i = 0, j = p.length - 2;
    double *A = p.A, pivot = A[p.length - 1], temp;
    do {
      while (A[i] < pivot) i++;
      while (A[j] > pivot && j > 0) j--;
      if (i < j) {
        temp = A[i];
        A[i] = A[j];
        A[j] = temp;
      }
    } while (i < j);
    A[p.length - 1] = A[i];
    A[i] = pivot;
    Problem subproblem1 = {A, i};
    return subproblem1;

  } else {
    int first_length = subproblems[0].length;
    Problem subproblem2 = {p.A + first_length + 1, p.length - first_length - 1};
    return subproblem2;
  }
}

Result merge(Result* results) {
  Result r1 = results[0];
  Result r2 = results[1];
  Result r = {r1.A, r1.length + r2.length + 1};
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
