#ifndef NODEEDITOR_H
#define NODEEDITOR_H

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <list>
#include "../interpreter/nodes/node.h"
#include <unordered_map>
using std::list;
using std::unordered_map;

struct UINode {
    int id;
    int inpinId;
    int outpinId;
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
    int getNewId();
    void addNode();
    void maybeChangeNodeMenuState();

private:
    int uniqueId = 0;
    list<UINode> nodeList;
    unordered_map<int, list<UINode>::iterator> nodeIdMap;
};

#endif