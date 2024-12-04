#ifndef SAMPLEBIASNODE_H
#define SAMPLEBIASNODE_H

#include "node.h"

class SampleBiasNode : public Node {
public:
    SampleBiasNode() {}
    void evaluate(Interpreter& interpreter) override;
};

#endif