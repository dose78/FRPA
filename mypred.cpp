#include "mypred.h"

bool CCW( Point *a, Point *b, Point *c ) {
  double pa[2] = {a->x, a->y};
  double pb[2] = {b->x, b->y};
  double pc[2] = {c->x, c->y};
  return ( orient2d(pa,pb,pc) > 0 );
}

bool leftOf( Point *a, QuadEdge *e ) {
  return CCW( a, e->Org(), e->Dest() );
}

bool rightOf( Point *a, QuadEdge *e ) {
  return CCW( a, e->Dest(), e->Org() );
}

bool inCircle( Point *a, Point *b, Point *c, Point *d ) {
  double pa[2] = {a->x, a->y};
  double pb[2] = {b->x, b->y};
  double pc[2] = {c->x, c->y};
  double pd[2] = {d->x, d->y};
  return ( incircle(pa,pb,pc,pd) > 0 );
}

