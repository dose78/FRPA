#include "DelaunayProblem.h"

DelaunayProblem::DelaunayProblem( std::vector<Point*> *S, int begin, int end ) {
  this->S = S;
  this->begin = begin;
  this->end = end;
}

bool DelaunayProblem::shouldRunBaseCase(int depth) {
  return (end-begin)<=3;
}

void DelaunayProblem::runBaseCase() {
  if( end-begin == 2 ) {
    Point *s1 = (*S)[begin];
    Point *s2 = (*S)[begin+1];
    QuadEdge *a = makeEdge();
    a->setOrg( s1 );
    a->setDest( s2 );
    this->ldo = a;
    this->rdo = a->Sym();
  } else if( end-begin == 3 ) {
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
      this->ldo = a;
      this->rdo = b->Sym();
    } else if( CCW( s1, s3, s2 ) ) {
      c = connect( b, a );
      this->ldo = c->Sym();
      this->rdo = c;
    } else {
      this->ldo = a;
      this->rdo = b->Sym();
    }
  } else {
    printf("Error, base case should have 2 or three points\n");
  }   
}

std::vector<Task*> DelaunayProblem::split() {
  int mid = begin + (end-begin)/2;
  Task *task1 = new Task( new DelaunayProblem( S, begin, mid ) );
  Task *task2 = new Task( new DelaunayProblem( S, mid, end ) );
  std::vector<Task*> tasks (2);
  tasks[0] = task1;
  tasks[1] = task2;
  return tasks;
}

void DelaunayProblem::merge( std::vector<Problem*> subproblems ) {
    DelaunayProblem* r1 = static_cast<DelaunayProblem*>(subproblems[0]);
    DelaunayProblem* r2 = static_cast<DelaunayProblem*>(subproblems[1]);
    QuadEdge *ldo = r1->ldo;
    QuadEdge *ldi = r1->rdo;
    QuadEdge *rdi = r1->ldo;
    QuadEdge *rdo = r1->rdo;

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
	while( inCircle( basel->Dest(), basel->Org(), lcand->Dest(), lcand->Onext()
			 ->Dest() ) ) {
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
    
    this->ldo = ldo;
    this->rdo = rdo;
}
