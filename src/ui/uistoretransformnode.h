#ifndef UISTORETRANSFORMNODE_H
#define UISTORETRANSFORMNODE_H  

#include "uinode.h"
#include "../interpreter/nodes/storetransformnode.h"

class UIStoreTransformNode : public UINode {
public:
    UIStoreTransformNode() = default; // cereal needs a default constructor
    UIStoreTransformNode(int id, int startPinId, int endPinId) : UINode("Store Transform", id, startPinId, endPinId) {}
    ~UIStoreTransformNode() = default;
    
    list<uPtr<Node>> toInterpreterNodes() override {
        list<uPtr<Node>> nodes;
        nodes.push_back(mkU<StoreTransformNode>());
        return nodes;
    }

    bool show() override {
        UINode::startShow();
        UINode::show();
        UINode::endShow();
        return false;
    }

    template<class Archive>
    void serialize(Archive & archive) {
        archive(cereal::base_class<UINode>(this));
    }

};

CEREAL_REGISTER_TYPE (UIStoreTransformNode)
CEREAL_REGISTER_POLYMORPHIC_RELATION(UINode, UIStoreTransformNode)

#endif // UISTORETRANSFORMNODE_H