#ifndef NODE_H
#define NODE_H

// Forward declare Interpreter
class Interpreter;

// Abstract base class for a command node
class Node {
public:
    virtual ~Node() = default;
    virtual void evaluate() = 0;

protected:
    static Interpreter& interpreter;
};

#endif
