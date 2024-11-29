#ifndef UISTORETRANSFORMNODE_H
#define UISTORETRANSFORMNODE_H  

#include "uinode.h"
#include "../interpreter/nodes/storetransformnode.h"

class UIStoreTransformNode : public UINode {
public:
    UIStoreTransformNode(int id, int startPinId, int endPinId) : UINode("Store Transform", id, startPinId, endPinId) {}
    ~UIStoreTransformNode() = default;
    
    list<uPtr<Node>> toInterpreterNodes() override {
        list<uPtr<Node>> nodes;
        nodes.push_back(mkU<StoreTransformNode>());
        return nodes;
    }

    bool show() override {
        UINode::show();
        return false;
    }

};

#endif // UISTORETRANSFORMNODE_H