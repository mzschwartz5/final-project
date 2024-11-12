#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <glm/glm.hpp>
#include "macros.h"
#include "nodes/node.h"

// VM to execute geometry skeleton generation commands
class Interpreter {

public:
    // Singleton for now. TODO: consider using service locator pattern.
    static Interpreter& get() {
        static Interpreter instance;
        return instance;
    }
    ~Interpreter() = default;

    void run();
    void push(uPtr<Node> node);
    uPtr<Node> pop();

private:
    Interpreter() {};
    static const int MAX_STACK_SIZE = 1000;
    int stackSize = 0;
    uPtr<Node> stack[MAX_STACK_SIZE];
};

#endif