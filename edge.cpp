#include "edge.h"

void QuadEdge::setOnext( QuadEdge *e ) { next = e; }
void QuadEdge::setRot( QuadEdge *e ) { rot = e; }

QuadEdge *QuadEdge::Rot() { return rot; }
QuadEdge *QuadEdge::Sym() { return rot->Rot(); }
QuadEdge *QuadEdge::Rotinv() { return rot->Rot()->Rot(); }
QuadEdge *QuadEdge::Onext() { return next; }
QuadEdge *QuadEdge::Oprev() { return rot->Onext()->Rot(); }
QuadEdge *QuadEdge::Lnext() { return Rotinv()->Onext()->Rot(); }
QuadEdge *QuadEdge::Lprev() { return next->Sym(); }
QuadEdge *QuadEdge::Rnext() { return rot->Onext()->Rotinv(); }
QuadEdge *QuadEdge::Rprev() { return Sym()->Onext(); }
QuadEdge *QuadEdge::Dnext() { return Sym()->Onext()->Sym(); }
QuadEdge *QuadEdge::Dprev() { return Rotinv()->Onext()->Rotinv(); }

void QuadEdge::setOrg( Point *p ) { org = p; }
void QuadEdge::setDest( Point *p ) { Sym()->setOrg(p); }
Point *QuadEdge::Org() { return org; };
Point *QuadEdge::Dest() { return Sym()->Org(); }

QuadEdge::QuadEdge() {}

QuadEdge *makeEdge() {
  QuadEdge *e0 = new QuadEdge();
  QuadEdge *e1 = new QuadEdge();
  QuadEdge *e2 = new QuadEdge();
  QuadEdge *e3 = new QuadEdge();

  e0->setRot( e1 );
  e1->setRot( e2 );
  e2->setRot( e3 );
  e3->setRot( e0 );

  e0->setOnext( e0 );
  e1->setOnext( e3 );
  e2->setOnext( e2 );
  e3->setOnext( e1 );

  return e0;
}

void splice( QuadEdge *a, QuadEdge *b ) {
  QuadEdge *alpha = a->Onext()->Rot();
  QuadEdge *beta = b->Onext()->Rot();
  
  QuadEdge *temp = a->Onext();
  a->setOnext( b->Onext() );
  b->setOnext( temp );

  temp = alpha->Onext();
  alpha->setOnext( beta->Onext() );
  beta->setOnext( temp );  
}

QuadEdge *connect( QuadEdge *a, QuadEdge *b ) {
  QuadEdge *e = makeEdge();
  e->setOrg( a->Dest() );
  e->setDest( b->Org() );
  splice( e, a->Lnext() );
  splice( e->Sym(), b );

  return e;
}

void deleteEdge( QuadEdge *e ) {
  splice( e, e->Oprev() );
  splice( e->Sym(), e->Sym()->Oprev() );

  delete e->Rotinv();
  delete e->Sym();
  delete e->Rot();
  delete e;
}

bool Lexico( const Point *p1, const Point *p2 ) {
  return( (p1->x < p2->x) || (p1->x == p2->x && p1->y < p2->y) );
}
bool RevLexico( const Point *p1, const Point *p2 ) {
  return( (p1->y < p2->y) || (p1->y == p2->y && p1->x < p2->x) );
}
