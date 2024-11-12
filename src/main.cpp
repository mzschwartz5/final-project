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
#include <list>

// Forward declarations
GLFWwindow* initializeGLFW();
void glfwErrorCallback(int error, const char* description);
void bindMouseInputsToWindow(GLFWwindow* window);

int main() {
    // Top level error handling
	try {
	    glfwSetErrorCallback(glfwErrorCallback);

		// GLFW initialization and global settings
		GLFWwindow* window = initializeGLFW();
		bindMouseInputsToWindow(window);
		stbi_set_flip_vertically_on_load(true); // global setting for STB image loader

		Turtle turtle;
		Interpreter interpreter(turtle);
		std::list<uPtr<Node>> nodeList;
		nodeList.push_back(mkU<NumberNode>(0.5f));
		nodeList.push_back(mkU<MoveNode>());

		interpreter.run(nodeList);

		// Main render loop
		// Draw images until told to explicitly stop (e.g. x out of window)
		while (!glfwWindowShouldClose(window))
		{
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			turtle.draw();
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
	//glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);

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

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // disable mouse for FPS camera system

	// Enable depth testing
	glEnable(GL_DEPTH_TEST);

	return window;
}

void glfwErrorCallback(int error, const char* description) {
    std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
}

void bindMouseInputsToWindow(GLFWwindow* window) {
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	// Lambda for mouse input callback
	auto mouseCallback = [](GLFWwindow* window, double xpos, double ypos) {
		static double lastX{ xpos };
		static double lastY{ ypos };

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) != GLFW_PRESS) {
			lastX = xpos;
			lastY = ypos;
			return;
		}

		double xoffset = xpos - lastX;
		double yoffset = lastY - ypos;
		lastX = xpos;
		lastY = ypos;

		Camera::getInstance().setCameraFront(xoffset, yoffset);
	};
	glfwSetCursorPosCallback(window, mouseCallback); // register callback

	// Call the lambda once to initialize the static camera pointer, so we can unbind it at the end of this function.
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	mouseCallback(window, xpos, ypos);

	// Lambda for scroll input callback
	auto scrollCallback = [](GLFWwindow* window, double xoffset, double yoffset) {
		float sensitivty = 1.5f;

		float fov = Camera::getInstance().getFov();
		fov -= (float)(yoffset * sensitivty);
		Camera::getInstance().setFov(fov);
	};
	glfwSetScrollCallback(window, scrollCallback); // register callback
	scrollCallback(window, 0.0, 0.0); // call once to initialize the static camera pointer, so we can then clear it.

	// Unbind GLFW user pointer
	glfwSetWindowUserPointer(window, nullptr);
}