#ifndef RESTORETRANSFORMNODE_H
#define RESTORETRANSFORMNODE_H

#include "node.h"

class RestoreTransformNode : public Node {
public:
    RestoreTransformNode() {}
    void evaluate(Interpreter& interpreter) override;
};

#endif // RESTORETRANSFORMNODE_H