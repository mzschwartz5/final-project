#ifndef SAMPLEDENSITYNODE_H
#define SAMPLEDENSITYNODE_H

#include "node.h"

class SampleDensityNode : public Node {
public:
    SampleDensityNode() {}
    void evaluate(Interpreter& interpreter) override;
};

#endif