#ifndef NODEEDITOR_H
#define NODEEDITOR_H

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <list>
#include "../interpreter/nodes/node.h"
#include <unordered_map>
#include "macros.h"
#include <string>
using std::list;
using std::unordered_map;
using std::string;

enum class NodeType {
    MOVE,
    RESTORE,
    STORE,
};

struct UINode {
    NodeType type;
    string name;
    int id;
    int inpinId;
    int outpinId;
};

// Factory class to create application nodes from UI nodes
class NodeTranslator {
public:
    NodeTranslator() = default;
    ~NodeTranslator() = default;
    list<uPtr<Node>> translate(list<UINode> nodeList);
};

class NodeEditor {

public:
    NodeEditor() = default;
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
    list<uPtr<Node>> getNodeList() { return translator.translate(nodeList); }
    bool isDirty() { return dirty; }

private:
    int getNewId();
    void addNode(NodeType nodeType, const string& name);
    void maybeChangeNodeMenuState();
    NodeTranslator translator;
    bool dirty = false;
    int uniqueId = 0;
    list<UINode> nodeList;
    unordered_map<int, list<UINode>::iterator> nodeIdMap;
};

#endif