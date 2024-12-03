#include "openglutils.h"
#include <iostream>
#include "glm/glm.hpp"

namespace OpenGLUtils {

GLFWwindow* initializeGLFW() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
#endif

    GLFWwindow* window = glfwCreateWindow(Constants::SCR_WIDTH, Constants::SCR_HEIGHT, "Spliced", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        std::exit(1);
    }
    glfwMakeContextCurrent(window);

    // GLFW dependent on GLAD - make sure glad is loaded
    if (!gladLoadGL(glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        std::exit(1);
    }

    // The reason we get the frame buffer here instead of using the width/height constants is because retina screens have a higher resolution than the window size.
    // After creating the window with our constants, we'll take what GLFW says the screen size is as the source of truth.
    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    glViewport(0, 0, screenWidth, screenHeight);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    return window;
}

void glfwErrorCallback(int error, const char* description) {
    std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
}

void bindMouseInputsToWindow(GLFWwindow* window, MouseCallbackContext context) {
    /*
        NOTE: GLFW callbacks must be function pointers. This is annoying because the callbacks need access to certain data like the camera. By passing in data to the lambda's capture, the lambda
        no longer decays to a function pointer. Instead, we have to use GLFW functions to store off and retrieve the data.
    */
    glfwSetWindowUserPointer(window, &context);

    // Lambda for mouse input callback
    // TODO - refactor this logic to be more readable (use a state pattern: https://gameprogrammingpatterns.com/state.html)
    auto mouseCallback = [](GLFWwindow* window, double xpos, double ypos) {
        static double lastX{ xpos };
        static double lastY{ ypos };
        static Camera* currentCamera{ nullptr };

        // Retrieve pointer to camera from GLFW and call method to set the camera front vector
        MouseCallbackContext* context = static_cast<MouseCallbackContext*>(glfwGetWindowUserPointer(window));
        static SplitViewport* splitViewport = context->splitViewport;
        static GLFWcursor* resizeCursor = context->resizeCursor;
        MouseMode mouseMode = splitViewport->getMouseMode();

        Viewport activeViewport = splitViewport->getActiveViewport(xpos);

        if (activeViewport == Viewport::BORDER || mouseMode == MouseMode::RESIZING) {
            glfwSetCursor(window, resizeCursor);
        }
        else {
            glfwSetCursor(window, NULL);
        }

        double xoffset = xpos - lastX;
        double yoffset = lastY - ypos;
        lastX = xpos;
        lastY = ypos;

        // Only switch the current camera if we've let go of the mouse button.
        if (!currentCamera) currentCamera = splitViewport->getCamera(activeViewport);

        if (mouseMode == MouseMode::PANNING) {
            if (activeViewport == Viewport::BORDER) return;
            currentCamera->pan(xoffset, yoffset);
            return;
        }

        if (mouseMode == MouseMode::ORBITING) {
            currentCamera->orbitCamera(xoffset, yoffset);
            return;
        }

        if (mouseMode == MouseMode::RESIZING) {
            splitViewport->setViewportSplitRatio(xpos / Constants::SCR_WIDTH);
            return;
        }

        if (mouseMode == MouseMode::NONE) {
            currentCamera = nullptr;
            return;
        }
    };
    glfwSetCursorPosCallback(window, mouseCallback); // register callback

    // Call the lambda once to initialize the context pointer, so we can unbind it at the end of this function.
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    mouseCallback(window, xpos, ypos);

    // Lambda for scroll input callback
    auto scrollCallback = [](GLFWwindow* window, double xoffset, double yoffset) {
        float sensitivty = 5.0f;
        MouseCallbackContext* context = static_cast<MouseCallbackContext*>(glfwGetWindowUserPointer(window));
        static SplitViewport* splitViewport = context->splitViewport;

        double cursorXPos, cursorYPos;
        glfwGetCursorPos(window, &cursorXPos, &cursorYPos);

        Viewport activeViewport = splitViewport->getActiveViewport(cursorXPos);
        Camera* pCamera = splitViewport->getCamera(activeViewport);

        pCamera->dolly(yoffset * sensitivty);
    };
    glfwSetScrollCallback(window, scrollCallback); // register callback
    scrollCallback(window, 0.0, 0.0); // call once to initialize the context pointer, so we can then clear it.

    auto mouseButtonCallback = [](GLFWwindow* window, int button, int action, int mods) {
        MouseCallbackContext* context = static_cast<MouseCallbackContext*>(glfwGetWindowUserPointer(window));
        static SplitViewport* splitViewport = context->splitViewport;

        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            splitViewport->setMouseMode(MouseMode::ORBITING);
        }
        else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
            splitViewport->setMouseMode(MouseMode::RESIZING);
        }
        else if (button = GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {
            splitViewport->setMouseMode(MouseMode::PANNING);
        }
        else if (action == GLFW_RELEASE) {
            splitViewport->setMouseMode(MouseMode::NONE);
        }
    };
    glfwSetMouseButtonCallback(window, mouseButtonCallback); // register callback
    mouseButtonCallback(window, 0, 0, 0); // call once to initialize the context pointer, so we can then clear it.

    // Unbind GLFW user pointer
    glfwSetWindowUserPointer(window, nullptr);
}

SplitViewport::SplitViewport(GLFWwindow* window) :
    viewports({
        {Viewport::LEFT, std::array<GLint, 4>{0, 0, static_cast<GLint>(Constants::SCR_WIDTH * viewportSplitRatio), Constants::SCR_HEIGHT}},
        {Viewport::RIGHT, std::array<GLint, 4>{static_cast<GLint>(Constants::SCR_WIDTH * viewportSplitRatio), 0, static_cast<GLint>(Constants::SCR_WIDTH * (1.0f - viewportSplitRatio)), Constants::SCR_HEIGHT}},
        {Viewport::BORDER, std::array<GLint, 4>{static_cast<GLint>(viewportSplitRatio * Constants::SCR_WIDTH - borderWidthPixels / 2), 0, borderWidthPixels, Constants::SCR_HEIGHT}}
    }),
    cameras({
        {Viewport::LEFT, Camera()},
        {Viewport::RIGHT, Camera()},
        {Viewport::BORDER, Camera()}
    })
{
    glfwSetWindowUserPointer(window, this);
    auto frameBufferReziseCallback = [](GLFWwindow* window, int width, int height) { 
        static SplitViewport* splitViewport = static_cast<SplitViewport*>(glfwGetWindowUserPointer(window));
        splitViewport->windowWidth = width;
        splitViewport->windowHeight = height;
        // Call this to set the widths
        splitViewport->setViewportSplitRatio(splitViewport->viewportSplitRatio);

        // Update the heights manually
        splitViewport->viewports[Viewport::LEFT][3] = height;
        splitViewport->viewports[Viewport::RIGHT][3] = height;
        splitViewport->viewports[Viewport::BORDER][3] = height;
    };
    glfwSetFramebufferSizeCallback(window, frameBufferReziseCallback);
    frameBufferReziseCallback(window, Constants::SCR_WIDTH, Constants::SCR_HEIGHT); // Call once to initialize the context pointer.

    glfwSetWindowUserPointer(window, nullptr);

    cameras[Viewport::LEFT].setScreenDims(viewports[Viewport::LEFT][2], viewports[Viewport::LEFT][3]);
    cameras[Viewport::RIGHT].setScreenDims(viewports[Viewport::RIGHT][2], viewports[Viewport::RIGHT][3]);
}

void SplitViewport::setViewport(Viewport viewportType) {
    const std::array<GLint, 4>& viewport = viewports[viewportType];
    glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
}

void SplitViewport::setViewportSplitRatio(double ratio) {
    viewportSplitRatio = glm::clamp(ratio, 0.1, 0.9);
    viewports[Viewport::LEFT][2] = static_cast<GLint>(windowWidth * viewportSplitRatio);
    viewports[Viewport::RIGHT][0] = static_cast<GLint>(windowWidth * viewportSplitRatio);
    viewports[Viewport::RIGHT][2] = static_cast<GLint>(windowWidth * (1.0f - viewportSplitRatio));
    viewports[Viewport::BORDER][0] = static_cast<GLint>(viewportSplitRatio * windowWidth - borderWidthPixels / 2);

    cameras[Viewport::LEFT].setScreenDims(viewports[Viewport::LEFT][2], viewports[Viewport::LEFT][3]);
    cameras[Viewport::RIGHT].setScreenDims(viewports[Viewport::RIGHT][2], viewports[Viewport::RIGHT][3]);
}

void SplitViewport::drawViewportBorder() {
    glScissor(viewports[Viewport::BORDER][0], viewports[Viewport::BORDER][1], viewports[Viewport::BORDER][2], viewports[Viewport::BORDER][3]);
    glEnable(GL_SCISSOR_TEST);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);
}

Viewport SplitViewport::getActiveViewport(double cursorPosX) const {
    if (cursorPosX < windowWidth * viewportSplitRatio - (borderWidthPixels + borderWidthBuffer) / 2) {
        return Viewport::LEFT;
    }
    else if (cursorPosX > windowWidth * viewportSplitRatio + (borderWidthPixels + borderWidthBuffer) / 2) {
        return Viewport::RIGHT;
    }

    return Viewport::BORDER;
}

}