#include "node.h"

class PitchNode : public Node {
public:
    PitchNode() {}
    void evaluate(Interpreter& interpreter) override;
};