#include "Task.h"
#include "Problem.h"
#include "edge.h"
#include "library.h"
#include "mypred.h"

class DelaunayProblem: public Problem {
public:
  std::vector<Point*> *S;
  int begin, end;
  QuadEdge *ldo, *rdo;
  DelaunayProblem(std::vector<Point*> *S, int begin, int end);
  bool shouldRunBaseCase(int depth);
  void runBaseCase();
  std::vector<Task*> split();
  void merge(std::vector<Problem*> subproblems);
};
