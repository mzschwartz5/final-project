#ifndef NUMBERNODE_H
#define NUMBERNODE_H
#include "node.h"

class NumberNode : public Node {
public:
    NumberNode(float value) : value(value) {}
    void evaluate() override;

private:
    float value;
};

#endif