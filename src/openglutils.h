#ifndef OPENGLUTILS_H
#define OPENGLUTILS_H

#include "camera.h"
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <unordered_map>
#include <array>
#include "constants.h"

namespace OpenGLUtils {

enum Viewport {
    LEFT,
    RIGHT,
    BORDER
};

class SplitViewport {

public:
    SplitViewport(Camera* cameraLeft, Camera* cameraRight);
    void setViewportSplitRatio(double ratio);
    void setViewport(Viewport viewPort);
    void drawViewportBorder();
    Viewport getActiveViewport(double cursorPosX) const;
    GLint getViewportWidth(Viewport viewPort) const { return viewports.at(viewPort)[2]; }
    GLint getViewportHeight(Viewport viewPort) const { return viewports.at(viewPort)[3]; }
    Camera* getCamera(Viewport viewPort) {
        return cameras[viewPort];
    }
private:
    float viewportSplitRatio = 0.5;
    int borderWidthPixels = 2;
    int borderWidthBuffer = 2; // (Invisible) buffer to make the border easier to click
    std::unordered_map<Viewport, std::array<GLint, 4>> viewports;
    std::unordered_map<Viewport, Camera*> cameras;
};

struct MouseCallbackContext {
    SplitViewport* splitViewport;
    GLFWcursor* resizeCursor;
};

GLFWwindow* initializeGLFW();
void glfwErrorCallback(int error, const char* description);
void bindMouseInputsToWindow(GLFWwindow* window, MouseCallbackContext data);


}

#endif // OPENGL_H