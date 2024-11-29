#include "interpreter.h"
#include "nodes/node.h"

bool Interpreter::_instantiated = false;

void Interpreter::run(const list<uPtr<Node>>& nodeList) {
    clearStack();
    turtle.reset();

    for (auto& node : nodeList) {
        node->evaluate(*this);
    }
    
    turtle.finalize();
}

void Interpreter::clearStack() {
    stackSize = 0;
}

void Interpreter::push(float value) {
    assert(stackSize < MAX_STACK_SIZE);
    stack[stackSize++] = value;
}

float Interpreter::pop() {
    assert(stackSize > 0);
    return stack[--stackSize];
}