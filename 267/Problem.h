#include <iostream>
#include <vector>
#include <cstdio>

#include <stdlib.h>
// #include <stdio.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <mkl.h>
// #include <cblas.h>

class Task;

#ifndef PROBLEM
#define PROBLEM

class Problem {
public:
    virtual bool shouldRunBaseCase(int depth) = 0;
    virtual void runBaseCase() = 0;
    virtual std::vector<Task*> split() = 0;
    virtual void merge(std::vector<Problem*> subproblems) = 0;
};

#endif
