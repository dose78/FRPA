#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <memory.h>
#include <string>


#ifndef FRAMEWORK
#define FRAMEWORK

#include "Problem.h"
#include "Task.h"

class Framework {
public:
    static void solve(Problem* problem, std::string customInterleaving);

private:
    static std::string interleaving;
    static void solve(Problem* problem, int depth);
    static bool shouldRunBaseCase(int depth);
    static void solveTask(Task* task, int depth);
    static void deleteSubproblems(std::vector<Problem*> subproblems);
    static std::vector<Problem*> getSubproblemsFromTasks(std::vector<Task*> tasks);
};

#endif
