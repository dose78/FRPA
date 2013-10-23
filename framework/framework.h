#ifndef FRAMEWORK
#define FRAMEWORK

#include "Problem.h"
#include "Task.h"
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>

class Framework {
public:
    static void solve(Problem* problem);

private:
    // static std::vector<int> interleaving;
    static void solve(Problem* problem, int depth);
    static bool shouldRunBaseCase(int depth);
    static void solveTask(Task* task, int depth);
    static void deleteSubproblems(std::vector<Problem*> subproblems);
    static std::vector<Problem*> getSubproblemsFromTasks(std::vector<Task*> tasks);
};

#endif
