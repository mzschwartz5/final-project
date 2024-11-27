#include "movenode.h"

void MoveNode::evaluate(Interpreter& interpreter) {
    int numArgs = static_cast<int>(interpreter.pop());

    if (numArgs == 1) { // Simple move
        interpreter.getTurtle().move(interpreter.pop());
    } else { // Move and scale, 4 args (distance, scale x, scale y, scale z)
        float scaleX = interpreter.pop();
        float scaleY = interpreter.pop();
        float scaleZ = interpreter.pop();
        float distance = interpreter.pop();
        interpreter.getTurtle().move(distance, glm::vec3{scaleX, scaleY, scaleZ});
    }
}