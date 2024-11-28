#ifndef NODEEDITOR_H
#define NODEEDITOR_H

#include <GLFW/glfw3.h>

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

private:

};

#endif