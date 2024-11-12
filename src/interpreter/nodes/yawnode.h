#ifndef YAWNODE_H
#define YAWNODE_H

#include "node.h"

class YawNode : public Node {
public:
    YawNode() {}
    void evaluate(Interpreter& interpreter) override;
};

#endif // YAWNODE_H