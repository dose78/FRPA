#include <cstring>
#include <cstddef>
#include <stdio.h>

#ifndef LIBRARY_H
#define LIBRARY_H

#define NUM_THREADS 2

double read_timer();

int find_option( int argc, char **argv, const char *option );
int read_int( int argc, char **argv, const char *option, int default_value );
char *read_string( int argc, char **argv, const char *option, char *default_value );
void skipcomments( FILE *f );

#endif 
