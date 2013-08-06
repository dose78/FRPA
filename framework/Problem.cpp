#include "Problem.h"
#include "Task.h"


std::vector<Problem*> Problem::splitSequential() {
    std::vector<Problem*> subproblems;
    std::vector<Task*> tasks = split();
    for(std::vector<Task*>::iterator taskIterator = tasks.begin(); taskIterator != tasks.end(); taskIterator++) {
        Task *task = *taskIterator;
        std::vector<Problem*> newSubproblems = task->getProblems();
        // subproblems.reserve(subproblems.size() + newSubproblems.size());
        subproblems.insert(subproblems.end(), newSubproblems.begin(), newSubproblems.end());
        delete task;
    }
    return subproblems;
};

void Problem::mergeSequential(std::vector<Problem*> subproblems) {
    merge(subproblems);
};

bool Problem::mustRunBaseCase() {
    return false;
}

bool Problem::canRunBaseCase() {
    return true;
}