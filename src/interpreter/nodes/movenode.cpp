#include "movenode.h"

void MoveNode::evaluate(Interpreter& interpreter) {
    interpreter.getTurtle().move(interpreter.pop());
}