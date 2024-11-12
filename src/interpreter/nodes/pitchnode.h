#ifndef PITCHNODE_H
#define PITCHNODE_H

#include "node.h"

class PitchNode : public Node {
public:
    PitchNode() {}
    void evaluate(Interpreter& interpreter) override;
};

#endif // PITCHNODE_H