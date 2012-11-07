#include "quicksort.h"
#include "sejits.h"

Problems split(Problem p) {
  Problems problems;
  problems.problems = malloc(2 * sizeof(Problem));
  problems.count = 2;

  int length = p.length, i = 0, j = length - 2;
  double *A = p.A, pivot = A[length - 1], temp;
  do {
    while (A[i] < pivot) i++;
    while (A[j] > pivot && j > 0) j--;
    if (i < j) {
      temp = A[i];
      A[i] = A[j];
      A[j] = temp;
    }
  } while (i < j);
  A[length - 1] = A[i];
  A[i] = pivot;
  Problem p1 = {A, i};
  Problem p2 = {A + i + 1, length - i - 1};
  problems.problems[0] = p1;
  problems.problems[1] = p2;
  return problems;
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
