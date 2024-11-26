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

    // Register callback on window resize
    auto frameBufferReziseCallback = [](GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); };
    glfwSetFramebufferSizeCallback(window, frameBufferReziseCallback);

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
        static bool resizingViewports{ false };

        // Retrieve pointer to camera from GLFW and call method to set the camera front vector
        MouseCallbackContext* context = static_cast<MouseCallbackContext*>(glfwGetWindowUserPointer(window));
        static SplitViewport* splitViewport = context->splitViewport;
        static GLFWcursor* resizeCursor = context->resizeCursor;

        double cursorXPos, cursorYPos;
        glfwGetCursorPos(window, &cursorXPos, &cursorYPos);

        Viewport activeViewport = splitViewport->getActiveViewport(cursorXPos);

        if (activeViewport == Viewport::BORDER || resizingViewports) {
            glfwSetCursor(window, resizeCursor);
        }
        else {
            glfwSetCursor(window, NULL);
        }

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) != GLFW_PRESS) {
            lastX = xpos;
            lastY = ypos;
            currentCamera = nullptr;
            resizingViewports = false;
            return;
        }

        double xoffset = xpos - lastX;
        double yoffset = lastY - ypos;
        lastX = xpos;
        lastY = ypos;

        // Mouse is in the border bar between the two viewports and we're not currently adjusting either camera.
        if (!currentCamera && (activeViewport == Viewport::BORDER || resizingViewports)) {
            splitViewport->setViewportSplitRatio(cursorXPos / Constants::SCR_WIDTH);
            resizingViewports = true;
            return;
        }

        // Only switch the current camera if we've let go of the mouse button.
        if (!currentCamera) currentCamera = splitViewport->getCamera(activeViewport);
        currentCamera->orbitCamera(xoffset, yoffset);
    };
    glfwSetCursorPosCallback(window, mouseCallback); // register callback

    // Call the lambda once to initialize the context pointer, so we can unbind it at the end of this function.
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    mouseCallback(window, xpos, ypos);

    // Lambda for scroll input callback
    auto scrollCallback = [](GLFWwindow* window, double xoffset, double yoffset) {
        float sensitivty = 1.5f;
        MouseCallbackContext* context = static_cast<MouseCallbackContext*>(glfwGetWindowUserPointer(window));
        static SplitViewport* splitViewport = context->splitViewport;

        double cursorXPos, cursorYPos;
        glfwGetCursorPos(window, &cursorXPos, &cursorYPos);

        Viewport activeViewport = splitViewport->getActiveViewport(cursorXPos);
        Camera* pCamera = splitViewport->getCamera(activeViewport);

        float fov = pCamera->getFov();
        fov -= (float)(yoffset * sensitivty);
        pCamera->setFov(fov);
    };
    glfwSetScrollCallback(window, scrollCallback); // register callback
    scrollCallback(window, 0.0, 0.0); // call once to initialize the context pointer, so we can then clear it.

    // Unbind GLFW user pointer
    glfwSetWindowUserPointer(window, nullptr);
}

SplitViewport::SplitViewport(Camera* const cameraLeft, Camera* const cameraRight) :
    viewports({
        {Viewport::LEFT, std::array<GLint, 4>{0, 0, static_cast<GLint>(Constants::SCR_WIDTH * viewportSplitRatio), Constants::SCR_HEIGHT}},
        {Viewport::RIGHT, std::array<GLint, 4>{static_cast<GLint>(Constants::SCR_WIDTH * viewportSplitRatio), 0, static_cast<GLint>(Constants::SCR_WIDTH * (1.0f - viewportSplitRatio)), Constants::SCR_HEIGHT}},
        {Viewport::BORDER, std::array<GLint, 4>{static_cast<GLint>(viewportSplitRatio * Constants::SCR_WIDTH - borderWidthPixels / 2), 0, borderWidthPixels, Constants::SCR_HEIGHT}}
    }),
    cameras({
        {Viewport::LEFT, cameraLeft},
        {Viewport::RIGHT, cameraRight},
        {Viewport::BORDER, nullptr}
    })
{
    cameraLeft->setScreenDims(viewports[Viewport::LEFT][2], viewports[Viewport::LEFT][3]);
    cameraRight->setScreenDims(viewports[Viewport::RIGHT][2], viewports[Viewport::RIGHT][3]);
}

void SplitViewport::setViewport(Viewport viewportType) {
    const std::array<GLint, 4>& viewport = viewports[viewportType];
    glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
}

void SplitViewport::setViewportSplitRatio(double ratio) {
    viewportSplitRatio = glm::clamp(ratio, 0.1, 0.9);
    viewports[Viewport::LEFT][2] = static_cast<GLint>(Constants::SCR_WIDTH * viewportSplitRatio);
    viewports[Viewport::RIGHT][0] = static_cast<GLint>(Constants::SCR_WIDTH * viewportSplitRatio);
    viewports[Viewport::RIGHT][2] = static_cast<GLint>(Constants::SCR_WIDTH * (1.0f - viewportSplitRatio));
    viewports[Viewport::BORDER][0] = static_cast<GLint>(viewportSplitRatio * Constants::SCR_WIDTH - borderWidthPixels / 2);
}

void SplitViewport::drawViewportBorder() {
    glScissor(viewports[Viewport::BORDER][0], viewports[Viewport::BORDER][1], viewports[Viewport::BORDER][2], viewports[Viewport::BORDER][3]);
    glEnable(GL_SCISSOR_TEST);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);
}

Viewport SplitViewport::getActiveViewport(double cursorPosX) const {
    if (cursorPosX < Constants::SCR_WIDTH * viewportSplitRatio - (borderWidthPixels + borderWidthBuffer) / 2) {
        return Viewport::LEFT;
    }
    else if (cursorPosX > Constants::SCR_WIDTH * viewportSplitRatio + (borderWidthPixels + borderWidthBuffer) / 2) {
        return Viewport::RIGHT;
    }

    return Viewport::BORDER;
}

}