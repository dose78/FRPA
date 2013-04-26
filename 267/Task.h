#include "Problem.h"

class Task {
public:
    std::vector<Problem*> problems;

    void addProblem(Problem p) {
        problems.push_back(p);
    }
};
