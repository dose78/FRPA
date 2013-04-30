#include "library.h"
#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>


double read_timer( )
{
  static bool initialized = false;
  static struct timeval start;
  struct timeval end;
  if( !initialized )
    {
      gettimeofday( &start, NULL );
      initialized = true;
    }

  gettimeofday( &end, NULL );

  return (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
}

int find_option( int argc, char **argv, const char *option )
{
  for( int i = 1; i < argc; i++ )
    if( strcmp( argv[i], option ) == 0 )
      return i;
  return -1;
}

int read_int( int argc, char **argv, const char *option, int default_value )
{
  int iplace = find_option( argc, argv, option );
  if( iplace >= 0 && iplace < argc-1 )
    return atoi( argv[iplace+1] );
  return default_value;
}

char *read_string( int argc, char **argv, const char *option, char *default_value )
{
  int iplace = find_option( argc, argv, option );
  if( iplace >= 0 && iplace < argc-1 )
    return argv[iplace+1];
  return default_value;
}

void skipcomments( FILE *f ) {
  char c1,c2;
  while( true) {
    int s = fscanf( f, "%1[#]%*[^\n]%[\n]", &c1, &c2 );
    if( s == 0 ) {
      // not a comment line
      break;
    }
    if( s == 1 ) {
      // line with only a #
      if( fscanf(f, "%1[\n]", &c1) == 0 ) {
	printf("Apparently this is a bug in my file parsing; try removing comments\n");
	exit(-1);
      }
      continue;
    }
  }
}
