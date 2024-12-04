#ifndef UI_SAMPLE_DENSITY_NODE_H
#define UI_SAMPLE_DENSITY_NODE_H

#include "uinode.h"
#include "../interpreter/nodes/sampledensitynode.h"
#include "../interpreter/nodes/numbernode.h"

class UISampleDensityNode : public UINode {
public:
    UISampleDensityNode() = default; // cereal needs a default constructor
    UISampleDensityNode(int id, int startPinId, int endPinId) : UINode("Sample Density", id, startPinId, endPinId) {}
    ~UISampleDensityNode() = default;
    list<uPtr<Node>> toInterpreterNodes() override {
        list<uPtr<Node>> nodes;
        nodes.push_back(mkU<NumberNode>(sampleDensity)); 
        nodes.push_back(mkU<SampleDensityNode>());
        return nodes;
    }
    bool show() override {
        UINode::startShow();
        UINode::show();
        bool dirty = false;

        if (ImGui::SliderInt("Sample Density", &sampleDensity, 1, 20)) {
            dirty = true;
        }

        UINode::endShow();
        return dirty;
    }

    template<class Archive>
    void serialize(Archive & archive) {
        archive(cereal::base_class<UINode>(this), sampleDensity);
    }

private:
    int sampleDensity = 10;

};

CEREAL_REGISTER_TYPE (UISampleDensityNode)
CEREAL_REGISTER_POLYMORPHIC_RELATION(UINode, UISampleDensityNode)

#endif // UI_SAMPLE_DENSITY_NODE_H