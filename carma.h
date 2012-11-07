typedef struct {
  int M, K, m, k, n, CM;
  double *A, *B, *C;
} Problem;

typedef struct {
  int m, n, CM;
  double *C;
} Result;