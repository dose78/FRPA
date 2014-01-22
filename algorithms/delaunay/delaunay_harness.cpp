#include "harness.h"
#include "framework.h"
#include "DelaunayProblem.h"

int main(int argc, char **argv) {
  std::string interleaving;
  if (argc > 1) {
      interleaving = argv[1];
  } else {
      interleaving = "";
  }

  for( int n = 1000; n <= 10000000; n *= 10 ) {

    std::vector<Point*> S;
    for( int i = 0; i < n; i++ ) {
      S.push_back( new Point );
      S.back()->index = i;
      S.back()->x = drand48();
      S.back()->y = drand48();
    }

    double stime = read_timer();

    DelaunayProblem *problem = new DelaunayProblem(&S, 0, n);

    Framework::solve(problem, interleaving);

    printf("%d time %f\n", n, read_timer()-stime);
    delete problem;
  }
}
