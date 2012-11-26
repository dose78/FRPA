#define MAX_DEPTH 5
#define SPLIT_M 1
#define SPLIT_K 2
#define SPLIT_N 3

typedef struct {
  int M, K, m, k, n, CM;
  double *A, *B, *C;
} Problem;

typedef struct {
  int m, n, CM;
  double *C;
} Result;