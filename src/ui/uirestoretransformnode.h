#ifndef UIRESTORETRANSFORMNODE_H
#define UIRESTORETRANSFORMNODE_H  

#include "uinode.h"
#include "../interpreter/nodes/restoretransformnode.h"

class UIRestoreTransformNode : public UINode {
public:
    UIRestoreTransformNode(int id, int startPinId, int endPinId) : UINode("Restore Transform", id, startPinId, endPinId) {}
    ~UIRestoreTransformNode() = default;
    
    list<uPtr<Node>> toInterpreterNodes() override {
        list<uPtr<Node>> nodes;
        nodes.push_back(mkU<RestoreTransformNode>());
        return nodes;
    }

    bool show() override {
        UINode::show();
        return false;
    }

};

#endif // UIRESTORETRANSFORMNODE_H