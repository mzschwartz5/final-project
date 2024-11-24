#include "storetransformnode.h"

void StoreTransformNode::evaluate(Interpreter& interpreter) {
    Turtle& turtle = interpreter.getTurtle();
    interpreter.push(turtle.getPitch());
    interpreter.push(turtle.getYaw());
    interpreter.push(turtle.getPosition().z);
    interpreter.push(turtle.getPosition().y);
    interpreter.push(turtle.getPosition().x);
    interpreter.push(turtle.getPositionIndex());
}