#ifndef EDGE_H
#define EDGE_H

struct Point {
  double x;
  double y;
  int index;
};

class QuadEdge {
 public:
  friend QuadEdge *makeEdge();

  void setOnext( QuadEdge *e );
  void setRot( QuadEdge *e );

  QuadEdge *Rot();
  QuadEdge *Sym();
  QuadEdge *Rotinv();
  QuadEdge *Onext();
  QuadEdge *Oprev();
  QuadEdge *Lnext();
  QuadEdge *Lprev();
  QuadEdge *Rnext();
  QuadEdge *Rprev();
  QuadEdge *Dnext();
  QuadEdge *Dprev();

  void setOrg( Point *p );
  void setDest( Point *p );
  Point *Org();
  Point *Dest();

 private:
  QuadEdge();
  QuadEdge( const QuadEdge& );
  QuadEdge& operator=( const QuadEdge& );
  QuadEdge *next;
  QuadEdge *rot;
  Point *org;
};

QuadEdge *makeEdge();
void splice( QuadEdge *a, QuadEdge *b );
QuadEdge *connect( QuadEdge *a, QuadEdge *b );
void deleteEdge( QuadEdge *e );
bool Lexico( const Point *p1, const Point *p2 );
bool RevLexico( const Point *p1, const Point *p2 );

#endif
