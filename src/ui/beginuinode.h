#ifndef BEGINUINODE_H
#define BEGINUINODE_H
#include "uinode.h"

class BeginUINode : public UINode {

public:
    BeginUINode() = default; // cereal needs a default constructor
    BeginUINode(int id, int startPinId, int endPinId) : UINode("Begin", id, startPinId, endPinId) {}
    ~BeginUINode() = default;
    
    list<uPtr<Node>> toInterpreterNodes() override {
        return {}; 
    }

    bool show() override {
        UINode::startShow();
        if (!created) {
            ImNodes::SetNodeGridSpacePos(id, ImVec2(0, 0));
            created = true;
        }

        ImNodes::BeginNodeTitleBar();
        ImGui::Text("%s", name.c_str());
        ImNodes::EndNodeTitleBar();
        ImNodes::BeginOutputAttribute(endPinId);
        ImGui::Text("out");
        ImNodes::EndOutputAttribute();
        UINode::endShow();
        return false;
    }

    template<class Archive>
    void serialize(Archive & archive) {
        archive(cereal::base_class<UINode>(this));
    }

private:
    bool created = false;

};  

CEREAL_REGISTER_TYPE (BeginUINode)
CEREAL_REGISTER_POLYMORPHIC_RELATION(UINode, BeginUINode)

#endif // BEGINUINODE_H