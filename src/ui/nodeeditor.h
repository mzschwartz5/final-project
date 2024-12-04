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
#include <cereal/archives/binary.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/memory.hpp>
#include <chrono>

using std::list;
using std::unordered_map;
using std::string;

struct Link {
    int id;
    int startPinId;
    int endPinId;
    int startNodeId;
    int endNodeId;

    template <class Archive>
    void serialize(Archive & ar) {
        ar(id, startPinId, endPinId, startNodeId, endNodeId);
    }
};

class NodeEditor {

public:
    NodeEditor(GLFWwindow* window, const string& fileNamePrefix = "");
    ~NodeEditor() = default;
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
    
    template <class Archive>
    void serialize(Archive & ar) {
        ar(uniqueId, beginNodeId, nodeIdMap, linkIdMap);
    }


private:
    int getNewId();
    void addNode(uPtr<UINode> node);
    void handleMenuChanges(bool linkDropped);
    bool shouldDeleteNode(int nodeId);
    void deleteNode(int nodeId);
    bool maybeAddLink();
    bool shouldDeleteLink(int linkId);
    void deleteLink(int linkId);

    void saveNodeEditor();
    void loadNodeEditor(const string& fileNamePrefix);

    string fileNamePrefix;
    int beginNodeId;
    bool createLinkWithNode = false;
    std::chrono::time_point<std::chrono::steady_clock> lastSaveTime;
    bool showSaveIndicator = false;
    int uniqueId = 0;
    bool dirty = true;
    unordered_map<int, uPtr<UINode>> nodeIdMap;
    unordered_map<int, Link> linkIdMap;
};

#endif