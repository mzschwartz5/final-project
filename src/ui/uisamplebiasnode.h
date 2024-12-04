#ifndef UI_SAMPLE_BIAS_NODE_H
#define UI_SAMPLE_BIAS_NODE_H

#include "uinode.h"
#include "../interpreter/nodes/samplebiasnode.h"
#include "../interpreter/nodes/numbernode.h"

class UISampleBiasNode : public UINode {
public:
    UISampleBiasNode() = default; // cereal needs a default constructor
    UISampleBiasNode(int id, int startPinId, int endPinId) : UINode("Sample Bias", id, startPinId, endPinId) {}
    ~UISampleBiasNode() = default;
    list<uPtr<Node>> toInterpreterNodes() override {
        list<uPtr<Node>> nodes;
        nodes.push_back(mkU<NumberNode>(sampleBias)); 
        nodes.push_back(mkU<SampleBiasNode>());
        return nodes;
    }
    bool show() override {
        UINode::startShow();
        UINode::show();
        bool dirty = false;

        if (ImGui::SliderFloat("Sample Bias", &sampleBias, 0.01f, 1.0f)) {
            dirty = true;
        }

        UINode::endShow();
        return dirty;
    }

    template<class Archive>
    void serialize(Archive & archive) {
        archive(cereal::base_class<UINode>(this), sampleBias);
    }

private:
    float sampleBias = 0.5f;

};

CEREAL_REGISTER_TYPE (UISampleBiasNode)
CEREAL_REGISTER_POLYMORPHIC_RELATION(UINode, UISampleBiasNode)

#endif // UI_SAMPLE_BIAS_NODE_H