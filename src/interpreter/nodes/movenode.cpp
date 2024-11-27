#include "movenode.h"

void MoveNode::evaluate(Interpreter& interpreter) {
    int numArgs = static_cast<int>(interpreter.pop());

    if (numArgs == 1) { // Simple move
        interpreter.getTurtle().move(interpreter.pop());
    } else { // Move and scale, 2 args
        float scale = interpreter.pop();
        float distance = interpreter.pop();
        interpreter.getTurtle().move(distance, scale);
    }
}