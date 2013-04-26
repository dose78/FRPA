#include "Problem.h"

#ifndef TASK
#define TASK

class Task {
    std::vector<Problem*> problems;

public:
    void addProblem(Problem* p) {
        problems.push_back(p);
    }

    std::vector<Problem*> getProblems() {
        return problems;
    }
};

#endif
