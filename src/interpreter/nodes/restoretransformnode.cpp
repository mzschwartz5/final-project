#include "restoretransformnode.h"

void RestoreTransformNode::evaluate(Interpreter& interpreter) {
    Turtle& turtle = interpreter.getTurtle();

    float scale = interpreter.pop();
    turtle.setPositionIndex(interpreter.pop());

    // Note: for functiosn with multiple arguments that need to be popped off the interpreter's stack,
    // the popping must be done outside of the function call, because there's no guarantee on the order of evaluation.
    float x = interpreter.pop();
    float y = interpreter.pop();
    float z = interpreter.pop();
    turtle.setState(glm::vec3{x, y, z}, scale, false);

    float yaw = interpreter.pop();
    float pitch = interpreter.pop();
    turtle.setRotation(
        yaw,
        pitch
    );
}