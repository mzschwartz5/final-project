#ifndef UI_MOVENODE_H
#define UI_MOVENODE_H

#include "uinode.h"
#include "../interpreter/nodes/movenode.h"
#include "../interpreter/nodes/numbernode.h"

class UIMoveNode : public UINode {
public:
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
        UINode::show();
        bool dirty = false;

        if (ImGui::SliderFloat("Distance", &distance, 0.0f, 1.0f)) {
            dirty = true;
        }

        if (ImGui::SliderFloat3("Scale", scale, 0.0f, 1.0f)) {
            dirty = true;
        }

        return dirty;
    }

private:
    float distance = 0;
    float scale[3] = {1.0f, 1.0f, 1.0f};
};
#endif