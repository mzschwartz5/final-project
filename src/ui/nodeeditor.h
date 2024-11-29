#ifndef NODEEDITOR_H
#define NODEEDITOR_H

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <list>
#include "../interpreter/nodes/node.h"
#include <unordered_map>
#include "macros.h"
#include <string>
#include "uinode.h"
#include "beginuinode.h"
using std::list;
using std::unordered_map;
using std::string;

struct Link {
    int id;
    int startPinId;
    int endPinId;
    int startNodeId;
    int endNodeId;
};

class NodeEditor {

public:
    NodeEditor();
    ~NodeEditor() = default;
    void init(GLFWwindow* window);
    void reset();
    void teardown();
    void show(
        int editorPosX,
        int editorPosY,
        int editorWidth,
        int editorHeight
    );
    list<uPtr<Node>> getNodeList() const;
    bool isDirty() { return dirty; }
    void setDirty(bool dirty) { this->dirty = dirty; }

private:
    int getNewId();
    void addNode(uPtr<UINode> node);
    void handleMenuChanges();
    bool shouldDeleteNode(int nodeId);
    void deleteNode(int nodeId);
    bool maybeAddLink();
    bool shouldDeleteLink(int linkId);
    void deleteLink(int linkId);
    int beginNodeId;
    int uniqueId = 0;
    bool dirty = false;
    list<uPtr<UINode>> nodeList;
    list<Link> linkList;
    unordered_map<int, list<uPtr<UINode>>::iterator> nodeIdMap;
    unordered_map<int, list<Link>::iterator> linkIdMap;
};

#endif