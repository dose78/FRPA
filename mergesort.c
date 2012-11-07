#include "header.h"
typedef struct {
  double *A;
  int length;
} Problem;
typedef struct {
  double *A;
  int length;
} Result;
#include "sejits.h"


Problems split(Problem p) {
  Problems problems;
  problems.problems = malloc(2 * sizeof(Problem));
  problems.count = 2;
  int midpoint = p.length/2;
  Problem p1 = {p.A, midpoint};
  Problem p2 = {p.A + midpoint, midpoint + (p.length % 2)};
  problems.problems[0] = p1;
  problems.problems[1] = p2;
  return problems;
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
