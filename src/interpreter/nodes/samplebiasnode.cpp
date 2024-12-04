#include "samplebiasnode.h"

void SampleBiasNode::evaluate(Interpreter& interpreter) {
    interpreter.getTurtle().setSampleBias(interpreter.pop());
}