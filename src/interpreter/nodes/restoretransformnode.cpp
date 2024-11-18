#include "restoretransformnode.h"

void RestoreTransformNode::evaluate(Interpreter& interpreter) {
    interpreter.getTurtle().setPositionIndex(interpreter.pop());
    // Note: for functiosn with multiple arguments that need to be popped off the interpreter's stack,
    // the popping must be done outside of the function call, because there's no guarantee on the order of evaluation.
    float x = interpreter.pop();
    float y = interpreter.pop();
    float z = interpreter.pop();
    interpreter.getTurtle().setPosition(glm::vec3{x, y, z}, false);

    float yaw = interpreter.pop();
    float pitch = interpreter.pop();
    interpreter.getTurtle().setRotation(
        yaw,
        pitch
    );
}