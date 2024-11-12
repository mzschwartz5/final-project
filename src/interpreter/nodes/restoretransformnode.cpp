#include "restoretransformnode.h"

void RestoreTransformNode::evaluate(Interpreter& interpreter) {
    interpreter.getTurtle().setPositionIndex(interpreter.pop());

    interpreter.getTurtle().setPosition(
        glm::vec3{
            interpreter.pop(), // x
            interpreter.pop(), // y
            interpreter.pop()  // z
        },
        false
    );

    interpreter.getTurtle().setRotation(
        interpreter.pop(), // yaw
        interpreter.pop()  // pitch
    );
}