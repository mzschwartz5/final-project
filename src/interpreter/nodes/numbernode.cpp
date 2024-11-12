#include "numbernode.h"

void NumberNode::evaluate(Interpreter& interpreter) {
    interpreter.push(value);
}