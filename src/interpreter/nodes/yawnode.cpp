#include "yawnode.h"

void YawNode::evaluate(Interpreter& interpreter) {
    interpreter.getTurtle().rotate(interpreter.pop(), 0.0f);
}