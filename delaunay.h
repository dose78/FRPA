#include "edge.h"
#include <vector>
#include <algorithm>
#include <utility>

QuadEdge *delaunay_wrapper( std::vector<Point*> *S, bool alternate );
std::pair<QuadEdge*,QuadEdge*> delaunay_horiz( std::vector<Point*> *S, int begin, int end );
std::pair<QuadEdge*,QuadEdge*> delaunay_alternate( std::vector<Point*> *S, int begin, int end, bool horiz );
