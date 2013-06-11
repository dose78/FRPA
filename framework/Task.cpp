#include "Task.h"
#include "Problem.h"

Task::Task(int size) {
    problems.reserve(size);
    oneElement = false;
}

Task::Task(Problem* p) {
    problems.reserve(1);
    problems.push_back(p);
    oneElement = true;
}

void Task::addProblem(Problem* p) {
    if (oneElement) {
        printf("WARNING: use Task(int size) constructor when adding multiple problems per task\n");
    }
    problems.push_back(p);
}

std::vector<Problem*> Task::getProblems() {
    return problems;
}
