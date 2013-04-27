#include "Problem.h"

#ifndef TASK
#define TASK

class Task {
    std::vector<Problem*> problems;

private:
    bool oneElement;

public:
    Task(int size) {
        problems.reserve(size);
        oneElement = false;
    }

    Task(Problem* p) {
        problems.reserve(1);
        problems.push_back(p);
        oneElement = true;
    }

    void addProblem(Problem* p) {
        if (oneElement) {
            printf("WARNING: use Task(int size) constructor when adding multiple problems per task\n");
        }
        problems.push_back(p);
    }

    std::vector<Problem*> getProblems() {
        return problems;
    }
};

#endif
