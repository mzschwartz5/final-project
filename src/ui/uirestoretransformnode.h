#ifndef UIRESTORETRANSFORMNODE_H
#define UIRESTORETRANSFORMNODE_H  

#include "uinode.h"
#include "../interpreter/nodes/restoretransformnode.h"

class UIRestoreTransformNode : public UINode {
public:
    UIRestoreTransformNode() = default; // cereal needs a default constructor
    UIRestoreTransformNode(int id, int startPinId, int endPinId) : UINode("Restore Transform", id, startPinId, endPinId) {}
    ~UIRestoreTransformNode() = default;
    
    list<uPtr<Node>> toInterpreterNodes() override {
        list<uPtr<Node>> nodes;
        nodes.push_back(mkU<RestoreTransformNode>());
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

CEREAL_REGISTER_TYPE (UIRestoreTransformNode)
CEREAL_REGISTER_POLYMORPHIC_RELATION(UINode, UIRestoreTransformNode)

#endif // UIRESTORETRANSFORMNODE_H