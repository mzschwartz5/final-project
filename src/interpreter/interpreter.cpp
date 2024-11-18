#include "interpreter.h"
#include "nodes/node.h"

bool Interpreter::_instantiated = false;

void Interpreter::run(const list<uPtr<Node>>& nodeList) {
    for (auto& node : nodeList) {
        node->evaluate(*this);
    }
}

void Interpreter::push(float value) {
    assert(stackSize < MAX_STACK_SIZE);
    stack[stackSize++] = value;
}

float Interpreter::pop() {
    assert(stackSize > 0);
    return stack[--stackSize];
}