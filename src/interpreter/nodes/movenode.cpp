#include "movenode.h"

void MoveNode::evaluate(Interpreter& interpreter) {
    Turtle& turtle = interpreter.getTurtle();
    float moveAmount = interpreter.pop();
    turtle.setPosition(turtle.getPosition() + vec3(0, moveAmount, 0));
}