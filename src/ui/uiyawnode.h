#ifndef UI_YAWNODE_H
#define UI_YAWNODE_H

#include "uinode.h"
#include <glm/glm.hpp>
#include "../interpreter/nodes/yawnode.h"
#include "../interpreter/nodes/numbernode.h"

class UIYawNode : public UINode {
public:
    UIYawNode() = default; // cereal needs a default constructor
    UIYawNode(int id, int startPinId, int endPinId) : UINode("Yaw", id, startPinId, endPinId) {}
    ~UIYawNode() = default;
    list<uPtr<Node>> toInterpreterNodes() override {
        list<uPtr<Node>> nodes;
        nodes.push_back(mkU<NumberNode>(glm::radians(yaw))); // 4 arguments for yaw node
        nodes.push_back(mkU<YawNode>());
        return nodes;
    }
    bool show() override {
        UINode::startShow();
        UINode::show();
        bool dirty = false;

        if (ImGui::SliderFloat("Yaw", &yaw, 0.0f, 360.0f)) {
            dirty = true;
        }

        UINode::endShow();
        return dirty;
    }

    template<class Archive>
    void serialize(Archive & archive) {
        archive(cereal::base_class<UINode>(this), yaw);
    }

private:
    float yaw = 0;
};

CEREAL_REGISTER_TYPE (UIYawNode)
CEREAL_REGISTER_POLYMORPHIC_RELATION(UINode, UIYawNode)

#endif