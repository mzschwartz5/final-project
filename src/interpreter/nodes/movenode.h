#ifndef MOVENODE_H
#define MOVENODE_H
#include "node.h"

class MoveNode : public Node {
public:
    MoveNode() {}
    void evaluate(Interpreter& interpreter) override;
};

#endif