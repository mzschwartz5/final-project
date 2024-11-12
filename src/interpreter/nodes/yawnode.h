#include "node.h"

class YawNode : public Node {
public:
    YawNode() {}
    void evaluate(Interpreter& interpreter) override;
};