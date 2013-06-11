#include <set>
#include <unordered_set>
#include <deque>
#include <algorithm>
#include "edge.h"
#include <stdio.h>

#ifndef TRIANGLE_H
#define TRIANGLE_H

struct Triangle {
  int p1;
  int p2;
  int p3;
};

template <class T>
inline void hash_combine(std::size_t& seed, const T& v)
{
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

namespace std {
  // needed to put Triangles in a std::unordered_set
  template <>
    struct hash<Triangle> {
    size_t operator() (const Triangle t ) const {
      int ta[3] = {t.p1, t.p2, t.p3};
      std::sort( ta, ta+3 );
      size_t ret = 0;
      hash_combine( ret, ta[0] );
      hash_combine( ret, ta[1] );
      hash_combine( ret, ta[2] );
      return ret;
    }
  };
  template <>
    struct equal_to<Triangle> {
    size_t operator() (const Triangle t1, const Triangle t2 ) const {
      int t1a[3] = {t1.p1, t1.p2, t1.p3};
      int t2a[3] = {t2.p1, t2.p2, t2.p3};
      std::sort( t1a, t1a+3 );
      std::sort( t2a, t2a+3 );
      return( t1a[0] == t2a[0] && t1a[1] == t2a[1] && t1a[2] == t2a[2] );
    }
  };

  // needed to put Triangles in a std::set
  template <>
    struct less<Triangle> {
    bool operator() (const Triangle t1, const Triangle t2 ) {
      int t1a[3] = {t1.p1, t1.p2, t1.p3};
      int t2a[3] = {t2.p1, t2.p2, t2.p3};
      std::sort( t1a, t1a+3 );
      std::sort( t2a, t2a+3 );
      if( t1a[0] < t2a[0] )
	return true;
      if( t1a[0] > t2a[0] )
	return false;
      if( t1a[1] < t2a[1] )
	return true;
      if( t1a[1] > t2a[1] )
	return false;
      if( t1a[2] < t2a[2] )
	return true;
      if( t1a[2] > t2a[2] )
	return false;
      return false;
    }
  };
}

void printTriangles( QuadEdge *e );
void writeTriangles( QuadEdge *e, FILE *f );
void checkTriangles( QuadEdge *e, FILE *f );

#endif
