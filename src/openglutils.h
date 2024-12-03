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

enum MouseMode {
    RESIZING,
    PANNING,
    ORBITING,
    NONE
};

class SplitViewport {

public:
    SplitViewport(GLFWwindow* window);
    float getViewportSplitRatio() const { return viewportSplitRatio; }
    void setViewportSplitRatio(double ratio);
    void setViewport(Viewport viewPort);
    void drawViewportBorder();
    Viewport getActiveViewport(double cursorPosX) const;
    GLint getViewportWidth(Viewport viewPort) const { return viewports.at(viewPort)[2]; }
    GLint getViewportHeight(Viewport viewPort) const { return viewports.at(viewPort)[3]; }
    int getBorderWidthPixels() const { return borderWidthPixels; }
    int getBorderWidthBuffer() const { return borderWidthBuffer; }
    Camera* getCamera(Viewport viewPort) {
        return &(cameras[viewPort]);
    }
    void setMouseMode(MouseMode mode) { mouseMode = mode; }
    MouseMode getMouseMode() const { return mouseMode; }

private:
    float windowWidth = Constants::SCR_WIDTH;
    float windowHeight = Constants::SCR_HEIGHT;
    float viewportSplitRatio = 0.5;
    int borderWidthPixels = 2;
    int borderWidthBuffer = 2; // (Invisible) buffer to make the border easier to click
    std::unordered_map<Viewport, std::array<GLint, 4>> viewports;
    std::unordered_map<Viewport, Camera> cameras;
    MouseMode mouseMode = MouseMode::NONE;
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