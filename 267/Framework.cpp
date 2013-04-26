#include "Framework.h"

void solve(Problem* problem, int depth);

void solveTask(Task* task, int depth) {
    std::vector<Problem*> problems = task->getProblems();
    for(std::vector<Problem*>::iterator problemIterator = problems.begin(); problemIterator != problems.end(); problemIterator++) {
        Problem *subproblem = *problemIterator;
        solve(subproblem, depth);
    }
}

std::vector<Problem*> getSubproblemsFromTasks(std::vector<Task*> tasks) {
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

void solve(Problem* problem, int depth) {
    if (problem->shouldRunBaseCase(depth)) {
        problem->runBaseCase();
        return;
    }

    std::vector<Task*> tasks = problem->split();

    // int BFS = depth % 2;
    int BFS = 1;
    if (BFS) {
        for(std::vector<Task*>::iterator taskIterator = tasks.begin(); taskIterator != tasks.end(); taskIterator++) {
            Task *task = *taskIterator;
            cilk_spawn solveTask(task, depth+1);
        }
        cilk_sync;
    } else {
        for(std::vector<Task*>::iterator taskIterator = tasks.begin(); taskIterator != tasks.end(); taskIterator++) {
            Task *task = *taskIterator;
            solveTask(task, depth+1);
        }
    }
    problem->merge(getSubproblemsFromTasks(tasks));
}

void solve(Problem* problem) {
    solve(problem, 0);
}
