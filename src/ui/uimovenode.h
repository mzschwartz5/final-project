#ifndef UI_MOVENODE_H
#define UI_MOVENODE_H

#include "uinode.h"
#include "../interpreter/nodes/movenode.h"
#include "../interpreter/nodes/numbernode.h"

class UIMoveNode : public UINode {
public:
    UIMoveNode() = default; // cereal needs a default constructor
    UIMoveNode(int id, int startPinId, int endPinId) : UINode("Move", id, startPinId, endPinId) {}
    ~UIMoveNode() = default;

    list<uPtr<Node>> toInterpreterNodes() override {
        list<uPtr<Node>> nodes;
        nodes.push_back(mkU<NumberNode>(distance));
        nodes.push_back(mkU<NumberNode>(scale[2]));
        nodes.push_back(mkU<NumberNode>(scale[1]));
        nodes.push_back(mkU<NumberNode>(scale[0]));
        nodes.push_back(mkU<NumberNode>(4.0)); // 4 arguments for move node
        nodes.push_back(mkU<MoveNode>());
        return nodes;
    }
    
    bool show() override {
        UINode::startShow();
        UINode::show();
        bool dirty = false;

        dirty = ( 
            ImGui::SliderFloat("Distance", &distance, 0.01f, 10.0f)
            || ImGui::SliderFloat("ScaleX", &scale[0], 0.01f, 5.0f)
            || ImGui::SliderFloat("ScaleY", &scale[1], 0.01f, 5.0f)
            || ImGui::SliderFloat("ScaleZ", &scale[2], 0.01f, 5.0f)
        );
        
        UINode::endShow();
        return dirty;
    }
    
    template<class Archive>
    void serialize(Archive & archive) {
        archive(cereal::base_class<UINode>(this), distance, scale);
    }

private:
    float distance = 0;
    float scale[3] = {1.0f, 1.0f, 1.0f};
};

CEREAL_REGISTER_TYPE (UIMoveNode)
CEREAL_REGISTER_POLYMORPHIC_RELATION(UINode, UIMoveNode)

#endif