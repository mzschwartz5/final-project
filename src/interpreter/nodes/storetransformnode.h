#ifndef STORETRANSFORMNODE_H
#define STORETRANSFORMNODE_H

#include "node.h"
#include "glm/glm.hpp"

class StoreTransformNode : public Node {
public:
    StoreTransformNode() {}
    void evaluate(Interpreter& interpreter) override;
};

#endif // STORETRANSFORMNODE_H