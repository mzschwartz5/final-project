#include "sampledensitynode.h"

void SampleDensityNode::evaluate(Interpreter& interpreter) {
    interpreter.getTurtle().setSampleDensity(interpreter.pop());
}