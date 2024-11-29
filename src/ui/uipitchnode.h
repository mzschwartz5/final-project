#ifndef UI_PITCHNODE_H
#define UI_PITCHNODE_H

#include "uinode.h"
#include <glm/glm.hpp>
#include "../interpreter/nodes/pitchnode.h"
#include "../interpreter/nodes/numbernode.h"

class UIPitchNode : public UINode {
public:
    UIPitchNode(int id, int startPinId, int endPinId) : UINode("Pitch", id, startPinId, endPinId) {}
    ~UIPitchNode() = default;
    list<uPtr<Node>> toInterpreterNodes() override {
        list<uPtr<Node>> nodes;
        nodes.push_back(mkU<NumberNode>(glm::radians(pitch))); // 4 arguments for pitch node
        nodes.push_back(mkU<PitchNode>());
        return nodes;
    }
    bool show() override {
        UINode::show();
        bool dirty = false;

        if (ImGui::SliderFloat("Pitch", &pitch, 0.0f, 360.0f)) {
            dirty = true;
        }

        return dirty;
    }

private:
    float pitch = 0;
};
#endif