#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <glm/glm.hpp>
#include "macros.h"
#include "turtle.h"
#include "macros.h"
#include <list>
using std::list;

// Forward declare Node
class Node;

// VM to execute geometry skeleton generation commands
class Interpreter {

public:
    // Note: single instance, but not a full-fledged singleton (no global access)
    Interpreter(Turtle& turtle) : turtle(turtle) {
        assert(!_instantiated);
        _instantiated = true;
    }

    ~Interpreter() {
        _instantiated = false;
    }

    void run(const list<uPtr<Node>>& nodeList);
    void push(float value);
    float pop();
    Turtle& getTurtle() { return turtle; }

private:
    static bool _instantiated;
    static const int MAX_STACK_SIZE = 1000;
    int stackSize = 0;
    float stack[MAX_STACK_SIZE];
    Turtle& turtle;
};

#endif