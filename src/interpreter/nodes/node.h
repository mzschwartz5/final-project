#ifndef NODE_H
#define NODE_H
#include "../interpreter.h"

// Abstract base class for a command node
class Node {
public:
    virtual ~Node() = default;
    virtual void evaluate(Interpreter& interpreter) = 0;
};

#endif
