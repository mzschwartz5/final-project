#include "interpreter.h"

void Interpreter::run() {
    while (stackSize > 0) {
        pop()->evaluate();
    }
}

void Interpreter::push(uPtr<Node> node) {
    assert(stackSize < MAX_STACK_SIZE);
    stack[stackSize++] = std::move(node);
}

uPtr<Node> Interpreter::pop() {
    assert(stackSize > 0);
    return std::move(stack[--stackSize]);
}