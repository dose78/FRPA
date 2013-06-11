#include "edge.h"

extern "C" {
  double orient2d(double*,double*,double*);
  double incircle(double*,double*,double*,double*);
  void exactinit();
}

bool CCW( Point *a, Point *b, Point *c );
bool leftOf( Point *a, QuadEdge *e );
bool rightOf( Point *a, QuadEdge *e );
bool inCircle( Point *a, Point *b, Point *c, Point *d );


