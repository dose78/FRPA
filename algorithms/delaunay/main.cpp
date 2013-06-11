#include "delaunay.h"
#include "library.h"
#include "triangles.h"
#include <stdlib.h>

int main( int argc, char **argv ) {

  int llen = read_int( argc, argv, "-l", 0 );
  int rlen = read_int( argc, argv, "-r", 0 );
  int seed = read_int( argc, argv, "-s", 0 );
  srand48(seed);
  char *input = read_string( argc, argv, "-i", NULL );
  if( !input && !llen && !rlen ) {
    printf("Specify an input .node file with the -i flag\n");
    exit(-1);
  }

  char *output = read_string( argc, argv, "-o", NULL );
  char *check = read_string( argc, argv, "-c", NULL );
  int time_only = read_int( argc, argv, "-t", 0 );
  bool alternate = (bool) read_int( argc, argv, "-a", 0 );

  double t0 = read_timer();

  std::vector<Point*> S;
  if( input ) {
    FILE *f = fopen( input, "r" );
    if( !f ) {
      printf("Error opening file: %s\n", input );
      exit(-1);
    }
    printf("Using input file %s\n", input);
    int n,d,a,b;
    int reads = 0;
    skipcomments(f);
    reads += fscanf( f, "%d %d %d %d\n", &n, &d, &a, &b );
    double junk;
    for( int i = 0; i < n; i++ ) {
      S.push_back( new Point );
      skipcomments(f);
      reads += fscanf( f, "%d %lf %lf", &(S.back()->index), &(S.back()->x), &(S.back()->y) );
      for( int j = 0; j < a+b; j++ )
	fscanf( f, "%lf", &junk );
      fscanf( f, "\n");
    }
    
    if( reads != 3*n+4 ) {
      printf("Failed to parse input file: %s\n", input );
      exit(-1);
    }
  } else if( rlen ) {
    printf("Using random data with %d points\n", rlen );
    for( int i = 0; i < rlen; i++ ) {
      S.push_back( new Point );
      S.back()->index = i;
      S.back()->x = drand48();
      S.back()->y = drand48();
    }
  } else if( llen ) {
    printf("Using linear data with %d points\n", llen );
    for( int i = 0; i < llen; i++ ) {
      S.push_back( new Point );
      S.back()->index = i;
      S.back()->x = i*1.;
      S.back()->y = 0.;    
    }
  }
    
  double t1 = read_timer();
  printf("read/generate time %f\n", t1-t0);
  if( alternate )
    printf("Using alternating horizontal and vertical cuts\n");
  else 
    printf("Using vertical cuts only\n");
  QuadEdge *e = delaunay_wrapper( &S, alternate );

  double t2 = read_timer();
  printf("compute time %f\n", t2-t1);

  if( !time_only ) {
    if( output ) {
      FILE *f = fopen( output, "w" );
      writeTriangles(e,f);
    } else if( check ) {
      FILE *f = fopen( check, "r" );
      checkTriangles(e,f);
    } else {
    printTriangles(e);
    }
  }
  
  double t3 = read_timer();
  printf("compare/print time %f\n", t3-t2);
}
