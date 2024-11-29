#ifndef UINODE_H
#define UINODE_H
#include <list>
#include <string>
#include "../interpreter/nodes/node.h"
#include "imnodes.h"
#include "imgui.h"
using std::string;

class UINode {

public:
    UINode(const string& name, int id, int startPinId, int endPinId) :
        name(name), id(id), startPinId(startPinId), endPinId(endPinId) {}
    
    ~UINode() = default;
    int getId() const { return id; }
    int getStartPinId() const { return startPinId; }
    int getEndPinId() const { return endPinId; }
    void setInLinkId(int inLinkId) { this->inLinkId = inLinkId; }
    void setOutLinkId(int outLinkId) { this->outLinkId = outLinkId; }
    int getInLinkId() const { return inLinkId; }
    int getOutLinkId() const { return outLinkId; }
    string getName() const { return name; }
    virtual list<uPtr<Node>> toInterpreterNodes() = 0;
    virtual bool show() {
        ImNodes::BeginNodeTitleBar();
        ImGui::Text("%s", name.c_str());
        ImNodes::EndNodeTitleBar();
        ImNodes::BeginInputAttribute(startPinId);
        ImGui::Text("in");
        ImNodes::EndInputAttribute();
        ImNodes::BeginOutputAttribute(endPinId);
        ImGui::Text("out");
        ImNodes::EndOutputAttribute();
        return false;
    }

protected:
    string name;
    int id;
    int startPinId;
    int endPinId;
    int inLinkId = -1;
    int outLinkId = -1;
};

#endif // UINODE_H