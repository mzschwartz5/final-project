#ifndef BEGINUINODE_H
#define BEGINUINODE_H
#include "uinode.h"

class BeginUINode : public UINode {

public:
    BeginUINode(int id, int startPinId, int endPinId) : UINode("Begin", id, startPinId, endPinId) {}
    ~BeginUINode() = default;
    
    list<uPtr<Node>> toInterpreterNodes() override {
        return {}; 
    }

    bool show() override {
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

        return false;
    }
private:
    bool created = false;

};  

#endif // BEGINUINODE_H