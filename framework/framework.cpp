#include "framework.h"
#include "debug.h"
#include <memory.h>

// std::vector<int> interleaving = ...;

bool Framework::shouldRunBaseCase(int depth) {
    // TODO something smart
    // int numCPU = sysconf( _SC_NPROCESSORS_ONLN );
    // return (depth >= (log2(numCPU)) - 0.01);
    return depth >= 2; // for strassen
}

void Framework::solveTask(Task* task, int depth) {
    std::vector<Problem*> problems = task->getProblems();
    for(std::vector<Problem*>::iterator problemIter = problems.begin(); problemIter != problems.end(); problemIter++) {
        Problem *subproblem = *problemIter;
        solve(subproblem, depth);
    }
}

void Framework::deleteSubproblems(std::vector<Problem*> subproblems) {
    for(std::vector<Problem*>::iterator problemIter = subproblems.begin(); problemIter != subproblems.end(); problemIter++) {
        Problem *subproblem = *problemIter;
        delete subproblem;
    }
}

std::vector<Problem*> Framework::getSubproblemsFromTasks(std::vector<Task*> tasks) {
    std::vector<Problem*> subproblems;
    for(std::vector<Task*>::iterator taskIterator = tasks.begin(); taskIterator != tasks.end(); taskIterator++) {
        Task *task = *taskIterator;
        std::vector<Problem*> newSubproblems = task->getProblems();
        // subproblems.reserve(subproblems.size() + newSubproblems.size());
        subproblems.insert(subproblems.end(), newSubproblems.begin(), newSubproblems.end());
        delete task;
    }
    return subproblems;
}

void Framework::solve(Problem* problem, int depth) {
    if (problem->canRunBaseCase() && (problem->mustRunBaseCase() || shouldRunBaseCase(depth))) {
        problem->runBaseCase();
        return;
    }

    std::vector<Problem*> subproblems;
    int BFS[] = { 1, 1, 1, 1, 1 };
    if (BFS[depth]) {
        std::vector<Task*> tasks = problem->split();
        for(std::vector<Task*>::iterator taskIterator = tasks.begin(); taskIterator != tasks.end(); taskIterator++) {
            Task *task = *taskIterator;
            cilk_spawn solveTask(task, depth+1);
        }
        cilk_sync;
        subproblems = getSubproblemsFromTasks(tasks);
        problem->merge(subproblems);
    } else {
        subproblems = problem->splitSequential();
        for(std::vector<Problem*>::iterator problemIter = subproblems.begin(); problemIter != subproblems.end(); problemIter++) {
            Problem *subproblem = *problemIter;
            solve(subproblem, depth + 1);
        }
        problem->mergeSequential(subproblems);
    }
    deleteSubproblems(subproblems);

    // Memory Tracking
    #ifdef DEBUG
        if (depth == 0) {
            printf("memory current: %d bytes\n", Memory::getMem());
            printf("memory max: %d bytes\n", Memory::getMax());
        }
    #endif
}

void Framework::solve(Problem* problem) {
    solve(problem, 0);
}
