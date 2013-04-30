#include "delaunay.h"
#include "mypred.h"
#include "triangles.h"

QuadEdge *delaunay_wrapper( std::vector<Point*> *S, bool alternate ) {
  exactinit();
  std::sort( S->begin(), S->end(), Lexico );
  std::vector<Point*> Sdedup;
  Sdedup.push_back( S->front() );
  for( auto it = S->begin()+1; it != S->end(); it++ ) {
    if( (*it)->x != Sdedup.back()->x || (*it)->y != Sdedup.back()->y )
      Sdedup.push_back( (*it) );
  }
  if( !alternate )
    return delaunay_horiz( &Sdedup, 0, Sdedup.size() ).first;
  return delaunay_alternate( &Sdedup, 0, Sdedup.size(), false ).first;
}

// the 2 point base case
inline std::pair<QuadEdge*,QuadEdge*> delaunay2( std::vector<Point*> *S, int begin, bool horiz ) {
  if( !horiz )
    std::sort( S->begin()+begin, S->begin()+begin+2, RevLexico );
  Point *s1 = (*S)[begin];
  Point *s2 = (*S)[begin+1];
  QuadEdge *a = makeEdge();
  a->setOrg( s1 );
  a->setDest( s2 );
  return std::make_pair( a, a->Sym() );
}

// the 3 point base case
inline std::pair<QuadEdge*,QuadEdge*> delaunay3( std::vector<Point*> *S, int begin, bool horiz ) {
  if( !horiz )
    std::sort( S->begin()+begin, S->begin()+begin+3, RevLexico );
  Point *s1 = (*S)[begin];
  Point *s2 = (*S)[begin+1];
  Point *s3 = (*S)[begin+2];
  QuadEdge *a = makeEdge();
  QuadEdge *b = makeEdge();
  QuadEdge *c;
  splice( a->Sym(), b );
  a->setOrg( s1 );
  a->setDest( s2 );
  b->setOrg( s2 );
  b->setDest( s3 );
  if( CCW( s1, s2, s3 ) ) {
    c = connect( b, a );
    return std::make_pair( a, b->Sym() );
  } else if( CCW( s1, s3, s2 ) ) {
    c = connect( b, a );
    return std::make_pair( c->Sym(), c );
  } else {
    return std::make_pair( a, b->Sym() );
  }
}

void delaunay_merge( QuadEdge *&ldi, QuadEdge *&ldo, QuadEdge *&rdi, QuadEdge *&rdo ) {
  // compute the lower common tangent
  while( true ) {
    if( leftOf( rdi->Org(), ldi ) )
      ldi = ldi->Lnext();
    else if( rightOf( ldi->Org(), rdi ) )
      rdi = rdi->Rprev();
    else
      break;
  }

  // create a first cross edge basel
  QuadEdge *basel = connect( rdi->Sym(), ldi );
  if( ldi->Org() == ldo->Org() )
    ldo = basel->Sym();
  if( rdi->Org() == rdo->Org() )
    rdo = basel;

  // main loop
  while( true ) {
    QuadEdge *lcand = basel->Sym()->Onext();
    if( rightOf( lcand->Dest(), basel ) ) {
      while( inCircle( basel->Dest(), basel->Org(), lcand->Dest(), lcand->Onext()->Dest() ) ) {
	QuadEdge *tmp = lcand->Onext();
	deleteEdge( lcand );
	lcand = tmp;
      }
    }

    QuadEdge *rcand = basel->Oprev();
    if( rightOf( rcand->Dest(), basel ) ) {
      while( inCircle( basel->Dest(), basel->Org(), rcand->Dest(), rcand->Oprev()->Dest() ) ) {
	QuadEdge *tmp = rcand->Oprev();
	deleteEdge( rcand );
	rcand = tmp;
      }
    }

    if( !rightOf(lcand->Dest(),basel) && !rightOf(rcand->Dest(),basel) )
      break;
    
    if( !rightOf(lcand->Dest(),basel) || (rightOf(rcand->Dest(),basel) && 
			  inCircle( lcand->Dest(), lcand->Org(), rcand->Org(), rcand->Dest() ) ) )
      basel = connect( rcand, basel->Sym() );
    else
      basel = connect( basel->Sym(), lcand->Sym() );
  }
}

// always splits horizontally; assumes that the points are presorted
std::pair<QuadEdge*,QuadEdge*> delaunay_horiz( std::vector<Point*> *S, int begin, int end ) {
  int n = end-begin;
  if( n == 2 )
    return delaunay2( S, begin, true );
  if( n == 3 )
    return delaunay3( S, begin, true );

  int mid = begin + n/2;
  auto ld = delaunay_horiz( S, begin, mid );
  auto rd = delaunay_horiz( S, mid, end );

  QuadEdge *ldo = ld.first;
  QuadEdge *ldi = ld.second;
  QuadEdge *rdi = rd.first;
  QuadEdge *rdo = rd.second;

  delaunay_merge( ldi, ldo, rdi, rdo );

  return std::make_pair( ldo, rdo );
}

Point *quickSelect( std::vector<Point*> *S, int i ) {
  int n = S->size();
  if( n == 1 )
    return (*S)[0];
  int pivot = (int)(drand48()*n);
  Point *p = (*S)[pivot];
  int numless = 0;
  for( auto it = S->begin(); it != S->end(); it++ ) {
    if( RevLexico( (*it), p ) )
      numless++;
  }
  if( numless > i ) {
    std::vector<Point*> Ss;
    for( auto it = S->begin(); it != S->end(); it++ ) {
      if( RevLexico( (*it), p ) )
	Ss.push_back( (*it) );
    }
    return quickSelect( &Ss, i );
  } else {
    std::vector<Point*> Sl;
    for( auto it = S->begin(); it != S->end(); it++ ) {
      if( !RevLexico( (*it), p ) )
	Sl.push_back( (*it) );
    }
    return quickSelect( &Sl, i-numless );
  }
}

// alternate split directions; assumes the points are always sorted lexicographically
// horiz specifies whether or not the previous split was horizontal
std::pair<QuadEdge*,QuadEdge*> delaunay_alternate( std::vector<Point*> *S, int begin, int end, bool horiz ) {
  int n = end-begin;
  if( n == 2 )
    return delaunay2( S, begin, horiz );
  if( n == 3 )
    return delaunay3( S, begin, horiz );

  if( horiz ) { 
    // this split should be vertical

    auto Scopy = std::vector<Point*>(S->begin()+begin,S->begin()+end);
    Point *pivot = quickSelect( &Scopy, n/2 );
    std::vector<Point*> *Sbot = new std::vector<Point*>;
    for( auto it = S->begin()+begin; it != S->begin()+end; it++ ) {
      if( RevLexico( (*it), pivot ) ) {
	Sbot->push_back( (*it) );
      }
    }
    auto bd = delaunay_alternate( Sbot, 0, Sbot->size(), false );
    delete Sbot;
    std::vector<Point*> *Stop = new std::vector<Point*>;
    for( auto it = S->begin()+begin; it != S->begin()+end; it++ ) {
      if( !RevLexico( (*it), pivot ) ) {
	Stop->push_back( (*it) );
      }
    }
    auto td = delaunay_alternate( Stop, 0, Stop->size(), false );
    delete Stop;

    QuadEdge *bdo = bd.first;
    QuadEdge *bdi = bd.second;
    QuadEdge *tdi = td.first;
    QuadEdge *tdo = td.second;
    
    delaunay_merge( bdi, bdo, tdi, tdo );

    // walk along the convex hull from tdo "forward" to find the rightmost and leftmost points
    Point *rp = tdo->Org();
    QuadEdge *ro = tdo;
    Point *lp = tdo->Org();
    QuadEdge *lo = tdo;
    QuadEdge *walk = tdo;
    do {
      walk = walk->Lnext();
      if( walk->Org()->x > rp->x || (walk->Org()->x == rp->x && walk->Org()->y > rp->y) ) {
	rp = walk->Org();
	ro = walk;
      }
      if( walk->Org()->x < lp->x || (walk->Org()->x == lp->x && walk->Org()->y < lp->y) ) {
	lp = walk->Org();
	lo = walk;
      }
    } while( walk->Org() != tdo->Org() );
    lo = lo->Lprev()->Sym();
    
    /*
    Point *rp = tdo->Org();
    QuadEdge *ro = tdo;
    QuadEdge *walk = tdo;
    do {
      walk = walk->Lnext();
      if( walk->Org()->x > rp->x || (walk->Org()->x == rp->x && walk->Org()->y > rp->y) ) {
	rp = walk->Org();
	ro = walk;
      }
    } while( walk->Org() != tdo->Org() );
    // walk along the convex hull from bdo "backward" to find the leftmost point
    Point *lp = bdo->Org();
    QuadEdge *lo = bdo;
    walk = bdo;
    do {
      walk = walk->Rprev();
      if( walk->Org()->x < lp->x || (walk->Org()->x == lp->x && walk->Org()->y < lp->y) ) {
	lp = walk->Org();
	lo = walk;
      }
    } while( walk->Org() != bdo->Org() );
    */
    return std::make_pair( lo, ro );
  } else {
    // this split should be horizontal

    int mid = begin + n/2;
    auto ld = delaunay_alternate( S, begin, mid, true );
    auto rd = delaunay_alternate( S, mid, end, true );

    
    QuadEdge *ldo = ld.first;
    QuadEdge *ldi = ld.second;
    QuadEdge *rdi = rd.first;
    QuadEdge *rdo = rd.second;

    delaunay_merge( ldi, ldo, rdi, rdo );

    
    Point *bp = ldo->Org();
    QuadEdge *bo = ldo;
    Point *tp = ldo->Org();
    QuadEdge *to = ldo;
    QuadEdge *walk = ldo;
    do {
      walk = walk->Rnext();
      if( walk->Org()->y < bp->y || (walk->Org()->y == bp->y && walk->Org()->x < bp->x) ) {
	bp = walk->Org();
	bo = walk;
      }
      if( walk->Org()->y > tp->y || (walk->Org()->y == tp->y && walk->Org()->x > tp->x) ) {
	tp = walk->Org();
	to = walk;
      }
    } while( walk->Org() != ldo->Org() );
    to = to->Rnext()->Sym();

    /*
    // walk along the convex hull from ldo "forward" to find the bottommost point
    Point *bp = ldo->Org();
    QuadEdge *bo = ldo;
    QuadEdge *walk = ldo;
    do {
      walk = walk->Rnext();
      if( walk->Org()->y < bp->y || (walk->Org()->y == bp->y && walk->Org()->x < bp->x) ) {
	bp = walk->Org();
	bo = walk;
      }
    } while( walk->Org() != ldo->Org() );

    // walk along the convex hull from rdo "backward" to find the topmost point
    Point *tp = rdo->Org();
    QuadEdge *to = rdo;
    walk = rdo;
    do {
      walk = walk->Lprev();
      if( walk->Org()->y > tp->y || (walk->Org()->y == tp->y && walk->Org()->x > tp->x) ) {
	tp = walk->Org();
	to = walk;
      }
    } while( walk->Org() != rdo->Org() );
    */

    return std::make_pair( bo, to );
  }
}
