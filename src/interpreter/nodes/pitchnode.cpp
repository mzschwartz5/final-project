#include "pitchnode.h"

void PitchNode::evaluate(Interpreter& interpreter) {
    interpreter.getTurtle().rotate(0.0f, interpreter.pop());
}