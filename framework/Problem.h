#include <iostream>
#include <vector>
#include <cstdio>

#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>

class Task;

#ifndef PROBLEM
#define PROBLEM

class Problem {
public:
    virtual bool shouldRunBaseCase(int depth);
    virtual void runBaseCase() = 0;
    virtual std::vector<Task*> split() = 0;
    virtual std::vector<Problem*> splitSequential();
    virtual void merge(std::vector<Problem*> subproblems) = 0;
    virtual void mergeSequential(std::vector<Problem*> subproblems);
};

#endif
