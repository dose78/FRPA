typedef struct {
  Problem* problems;
  int count;
} Problems;

// Methods to be implemented
Problems split(Problem p);
Result merge(Result* results);
int should_run_base_case(Problem problem, int depth);
Result base_case(Problem p);

Result solve(Problem problem, int depth) {
  if (should_run_base_case(problem, depth)) {
    return base_case(problem);
  }

  Problems subproblems = split(problem);
  Result* results = malloc(subproblems.count * sizeof(Result));

  int i;
  for (i = 0; i < subproblems.count; i++) {
    results[i] = cilk_spawn solve(subproblems.problems[i], depth + 1);
  }
  cilk_sync;

  free(subproblems.problems);

  Result result = merge(results);
  free(results);
  return result;
}
