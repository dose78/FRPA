#include "triangles.h"
#include "mypred.h"
#include "library.h"
#include <stdio.h>

// find all the triangles by a DFS from e, checking both sides of each edge.
// This might try to add each triangle as many as 2 times, 
// but it is easy and clearly correct.
std::unordered_set<Triangle> *findTriangles( QuadEdge *e ) {
  std::unordered_set<Triangle> *ret = new std::unordered_set<Triangle>;
  std::deque<QuadEdge*> queue;
  std::unordered_set<QuadEdge*> closed_list;

  queue.push_back(e);

  while( queue.size() ) {
    QuadEdge *edge = queue.back();
    queue.pop_back();
    if( closed_list.find(edge) != closed_list.end() )
      continue;
    closed_list.insert(edge);
    //queue.push_back( edge->Sym() );
    queue.push_back( edge->Onext() );
    queue.push_back( edge->Dnext() );
    
    // find the triangle to the left of edge
    Point *p1 = edge->Org();
    Point *p2 = edge->Dest();
    QuadEdge *edge2 = edge->Lnext();
    Point *p3 = edge2->Dest();
    QuadEdge *edge3 = edge2->Lnext();
    // verify the triangle is a triangle
    if( p1 == p2 || p1 == p3 || p2 == p3 || edge == edge2 || edge == edge3 || edge2 == edge3 || edge3->Dest() != p1 )
      continue;
    // only add it if it is in CCW order; otherwise it is the outside triangle
    if( CCW( p1, p2, p3 ) ) {
      Triangle t;
      t.p1 = p1->index;
      t.p2 = p2->index;
      t.p3 = p3->index;
      ret->insert(t);
    }
  }
  return ret;
}

void printTriangles( QuadEdge *e ) {
  auto triangles = findTriangles(e);
  int i = 1;
  for( auto it = triangles->begin(); it != triangles->end(); it++, i++ )
    printf("%d %d %d %d\n", i, it->p1, it->p2, it->p3 );
}

void writeTriangles( QuadEdge *e, FILE *f ) {
  auto triangles = findTriangles(e);
  int i = 1;
  fprintf(f, "%lu %d %d\n", triangles->size(), 3, 0 );
  for( auto it = triangles->begin(); it != triangles->end(); it++, i++ )
    fprintf(f, "%d %d %d %d\n", i, it->p1, it->p2, it->p3 );  
}

void checkTriangles( QuadEdge *e, FILE *f ) {
  bool correct = true;
  auto triangles = findTriangles(e);
  int n,a,attributes;
  int reads = 0;
  fscanf(f, "%d %d %d\n", &n, &a, &attributes );
  if( n != triangles->size() ) {
    printf("Mismatch in number of triangles %d %lu\n", n, triangles->size() );
    correct = false;
  }
  for( int i = 0; i < n; i++ ) {
    int a,b,c,d;
    double junk;
    reads += fscanf( f, "%d %d %d %d", &a, &b, &c, &d );
    for( int j = 0; j < attributes; j++ )
      fscanf( f, "%lf", &junk );
    fscanf( f, "\n" );
    Triangle t;
    t.p1 = b;
    t.p2 = c;
    t.p3 = d;
    if( triangles->find(t) == triangles->end() ) {
      correct = false;
      printf("Triangle %d %d %d %d in file but not in triangulation\n", a,b,c,d );
    }
  }
  if( correct )
    printf("Computed triangulation matches file.\n");
  else
    printf("Warning: computed triangulation differs from file.  See discrepancies above.\n");
}
