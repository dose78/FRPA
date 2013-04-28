#include "Problem.h"

#ifndef TASK
#define TASK

class Task {
private:
    std::vector<Problem*> problems;
    bool oneElement;

public:
    Task(int size);
    Task(Problem* p);
    void addProblem(Problem* p);
    std::vector<Problem*> getProblems();
};

#endif
