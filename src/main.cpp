#include <glad/gl.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <macros.h>
#include "constants.h"
#include "camera.h"
#include "interpreter/turtle.h"
#include "interpreter/interpreter.h"
#include "interpreter/nodes/movenode.h"
#include "interpreter/nodes/numbernode.h"
#include "interpreter/nodes/yawnode.h"
#include "interpreter/nodes/storetransformnode.h"
#include "interpreter/nodes/restoretransformnode.h"
#include <list>

// Forward declarations
struct MouseCallbackContext {
	Camera* cameraLeft;
	Camera* cameraRight;
	float& windowSplitRatio;
	GLFWcursor* resizeCursor;
};

GLFWwindow* initializeGLFW();
void glfwErrorCallback(int error, const char* description);
void bindMouseInputsToWindow(GLFWwindow* window, MouseCallbackContext data);

int main() {
    // Top level error handling
	try {
	    glfwSetErrorCallback(glfwErrorCallback);

		// GLFW initialization and global settings
		GLFWwindow* window = initializeGLFW();
		float windowSplitRatio = 0.5f;
		Camera cameraLeft;
		Camera cameraRight;
		GLFWcursor* resizeCursor = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
		bindMouseInputsToWindow(window, { &cameraLeft, &cameraRight, windowSplitRatio, resizeCursor});
		stbi_set_flip_vertically_on_load(true); // global setting for STB image loader

		Turtle turtle;
		Interpreter interpreter(turtle);
		std::list<uPtr<Node>> nodeList;
		nodeList.push_back(mkU<StoreTransformNode>());
		nodeList.push_back(mkU<NumberNode>(5.0f));
		nodeList.push_back(mkU<MoveNode>());
		nodeList.push_back(mkU<RestoreTransformNode>());
		nodeList.push_back(mkU<NumberNode>(M_PI / 4.0f));
		nodeList.push_back(mkU<YawNode>());
		nodeList.push_back(mkU<StoreTransformNode>());
		nodeList.push_back(mkU<NumberNode>(5.0f));
		nodeList.push_back(mkU<MoveNode>());
		nodeList.push_back(mkU<RestoreTransformNode>());
		nodeList.push_back(mkU<NumberNode>(M_PI / 4.0f));
		nodeList.push_back(mkU<YawNode>());
		nodeList.push_back(mkU<StoreTransformNode>());
		nodeList.push_back(mkU<NumberNode>(5.0f));
		nodeList.push_back(mkU<MoveNode>());
		nodeList.push_back(mkU<RestoreTransformNode>());
		nodeList.push_back(mkU<NumberNode>(M_PI / 4.0f));
		nodeList.push_back(mkU<YawNode>());
		nodeList.push_back(mkU<StoreTransformNode>());
		nodeList.push_back(mkU<NumberNode>(5.0f));
		nodeList.push_back(mkU<MoveNode>());
		nodeList.push_back(mkU<RestoreTransformNode>());
		nodeList.push_back(mkU<NumberNode>(M_PI / 4.0f));
		nodeList.push_back(mkU<YawNode>());
		nodeList.push_back(mkU<StoreTransformNode>());
		nodeList.push_back(mkU<NumberNode>(5.0f));
		nodeList.push_back(mkU<MoveNode>());
		nodeList.push_back(mkU<RestoreTransformNode>());
		nodeList.push_back(mkU<NumberNode>(M_PI / 4.0f));
		nodeList.push_back(mkU<YawNode>());
		nodeList.push_back(mkU<StoreTransformNode>());
		nodeList.push_back(mkU<NumberNode>(5.0f));
		nodeList.push_back(mkU<MoveNode>());
		nodeList.push_back(mkU<RestoreTransformNode>());
		nodeList.push_back(mkU<NumberNode>(M_PI / 4.0f));
		nodeList.push_back(mkU<YawNode>());
		nodeList.push_back(mkU<StoreTransformNode>());
		nodeList.push_back(mkU<NumberNode>(5.0f));
		nodeList.push_back(mkU<MoveNode>());
		nodeList.push_back(mkU<RestoreTransformNode>());
		nodeList.push_back(mkU<NumberNode>(M_PI / 4.0f));
		nodeList.push_back(mkU<YawNode>());
		nodeList.push_back(mkU<StoreTransformNode>());
		nodeList.push_back(mkU<NumberNode>(5.0f));
		nodeList.push_back(mkU<MoveNode>());
		nodeList.push_back(mkU<RestoreTransformNode>());
		nodeList.push_back(mkU<NumberNode>(M_PI / 4.0f));
		nodeList.push_back(mkU<YawNode>());

		interpreter.run(nodeList);

		// Main render loop
		// Draw images until told to explicitly stop (e.g. x out of window)
		while (!glfwWindowShouldClose(window))
		{
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			// Render left viewport
			glViewport(0, 0, windowSplitRatio * Constants::SCR_WIDTH, Constants::SCR_HEIGHT);
			turtle.draw(cameraLeft.calcViewMatrix(), cameraLeft.calcProjectionMatrix(Constants::SCR_WIDTH * windowSplitRatio, Constants::SCR_HEIGHT));
			// Render right viewport
			glViewport(windowSplitRatio * Constants::SCR_WIDTH, 0, (1.0f - windowSplitRatio) * Constants::SCR_WIDTH, Constants::SCR_HEIGHT);
			turtle.draw(cameraRight.calcViewMatrix(), cameraRight.calcProjectionMatrix(Constants::SCR_WIDTH * (1.0f - windowSplitRatio), Constants::SCR_HEIGHT));

			// Render a border bar between the two viewports using glScissor
			glViewport(0, 0, Constants::SCR_WIDTH, Constants::SCR_HEIGHT);
			glEnable(GL_SCISSOR_TEST);
			glScissor(windowSplitRatio * Constants::SCR_WIDTH - Constants::VIEWPORT_BORDER_WIDTH / 2, 0, Constants::VIEWPORT_BORDER_WIDTH, Constants::SCR_HEIGHT);
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			glDisable(GL_SCISSOR_TEST);

			glfwSwapBuffers(window);
			glfwPollEvents(); // updates window state upon events like keyboard or mouse inputs;
		}

		glfwTerminate(); // clean up GLFW resources
	}
	catch (const char* str) {
		std::cerr << str << std::endl;
	}

	return 0;
}

GLFWwindow* initializeGLFW() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
#endif

	GLFWwindow* window = glfwCreateWindow(Constants::SCR_WIDTH, Constants::SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
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

void bindMouseInputsToWindow(GLFWwindow* window, MouseCallbackContext data) {
	/*
		NOTE: GLFW callbacks must be function pointers. This is annoying because the callbacks need access to certain data like the camera. By passing in data to the lambda's capture, the lambda
		no longer decays to a function pointer. Instead, we have to use GLFW functions to store off and retrieve the data.
	*/
	glfwSetWindowUserPointer(window, &data);

	// Lambda for mouse input callback
	// TODO - refactor this logic to be more readable
	auto mouseCallback = [](GLFWwindow* window, double xpos, double ypos) {
		static double lastX{ xpos };
		static double lastY{ ypos };
		static Camera* currentCamera{ nullptr };
		static bool resizingViewports{ false };

		// Retrieve pointer to camera from GLFW and call method to set the camera front vector
		static MouseCallbackContext* context = static_cast<MouseCallbackContext*>(glfwGetWindowUserPointer(window));

		double cursorXPos, cursorYPos;
		glfwGetCursorPos(window, &cursorXPos, &cursorYPos);

		if ((cursorXPos > context->windowSplitRatio * Constants::SCR_WIDTH - Constants::VIEWPORT_BORDER_WIDTH / 2 &&
			cursorXPos < context->windowSplitRatio * Constants::SCR_WIDTH + Constants::VIEWPORT_BORDER_WIDTH / 2) ||
			resizingViewports) {
			glfwSetCursor(window, context->resizeCursor);
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

		// Mouse is in the border bar between the two viewports
		if ((cursorXPos > context->windowSplitRatio * Constants::SCR_WIDTH - Constants::VIEWPORT_BORDER_WIDTH / 2 &&
			cursorXPos < context->windowSplitRatio * Constants::SCR_WIDTH + Constants::VIEWPORT_BORDER_WIDTH / 2) ||
			resizingViewports) {
			context->windowSplitRatio = glm::clamp((float)cursorXPos / Constants::SCR_WIDTH, 0.1f, 0.9f);
			resizingViewports = true;
			return;
		}

		if (!currentCamera && cursorXPos < Constants::SCR_WIDTH * context->windowSplitRatio) {
			currentCamera = context->cameraLeft;
		}
		else if (!currentCamera) {
			currentCamera = context->cameraRight;
		}

		currentCamera->setCameraFront(xoffset, yoffset);
	};
	glfwSetCursorPosCallback(window, mouseCallback); // register callback

	// Call the lambda once to initialize the context pointer, so we can unbind it at the end of this function.
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	mouseCallback(window, xpos, ypos);

	// Lambda for scroll input callback
	auto scrollCallback = [](GLFWwindow* window, double xoffset, double yoffset) {
		float sensitivty = 1.5f;
		static MouseCallbackContext* context = static_cast<MouseCallbackContext*>(glfwGetWindowUserPointer(window));

		Camera* pCamera = context->cameraRight;
		double cursorXPos, cursorYPos;

		glfwGetCursorPos(window, &cursorXPos, &cursorYPos);
		if (cursorXPos < Constants::SCR_WIDTH * context->windowSplitRatio) {
			pCamera = context->cameraLeft;
		}

		float fov = pCamera->getFov();
		fov -= (float)(yoffset * sensitivty);
		pCamera->setFov(fov);
	};
	glfwSetScrollCallback(window, scrollCallback); // register callback
	scrollCallback(window, 0.0, 0.0); // call once to initialize the context pointer, so we can then clear it.

	// Unbind GLFW user pointer
	glfwSetWindowUserPointer(window, nullptr);
}