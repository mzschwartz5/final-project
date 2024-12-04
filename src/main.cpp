#include <glad/gl.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <macros.h>
#include "constants.h"
#include "interpreter/turtle.h"
#include "interpreter/interpreter.h"
#include "interpreter/nodes/movenode.h"
#include "interpreter/nodes/numbernode.h"
#include "interpreter/nodes/yawnode.h"
#include "interpreter/nodes/storetransformnode.h"
#include "interpreter/nodes/restoretransformnode.h"
#include <list>
#include "openglutils.h"
#include "cube.h"
#include "ui/nodeeditor.h"

int main() {
    // Top level error handling
	try {
	    glfwSetErrorCallback(OpenGLUtils::glfwErrorCallback);

		// GLFW initialization and global settings
		GLFWwindow* window = OpenGLUtils::initializeGLFW();
		OpenGLUtils::SplitViewport splitViewport(window);
		GLFWcursor* resizeCursor = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
		OpenGLUtils::MouseCallbackContext context{ &splitViewport, resizeCursor };
		OpenGLUtils::bindMouseInputsToWindow(window, context);
		stbi_set_flip_vertically_on_load(true); // global setting for STB image loader

		Turtle turtle;
		Interpreter interpreter(turtle);

		NodeEditor nodeEditor;
		nodeEditor.init(window);

		// Main render loop
		// Draw images until told to explicitly stop (e.g. x out of window)
		while (!glfwWindowShouldClose(window))
		{
			if (nodeEditor.isDirty()) {
				interpreter.run(nodeEditor.getNodeList());
				nodeEditor.setDirty(false);
			}

			nodeEditor.reset();
			
			glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			// Render left viewport
			splitViewport.setViewport(OpenGLUtils::Viewport::LEFT);
			turtle.drawGeometry(
				splitViewport.getCamera(OpenGLUtils::Viewport::LEFT)->calcViewMatrix(),
				glm::vec2(splitViewport.getViewportWidth(OpenGLUtils::Viewport::LEFT), splitViewport.getViewportHeight(OpenGLUtils::Viewport::LEFT)),
				0.0f,
				splitViewport.getCamera(OpenGLUtils::Viewport::LEFT)->getNearPlaneDims(),
				splitViewport.getCamera(OpenGLUtils::Viewport::LEFT)->getNearPlane()
			);

			splitViewport.setViewport(OpenGLUtils::Viewport::RIGHT);

			// Render a border bar between the two viewports
			splitViewport.setViewport(OpenGLUtils::Viewport::BORDER);
			splitViewport.drawViewportBorder();

			nodeEditor.show(
				splitViewport.getViewportWidth(OpenGLUtils::Viewport::LEFT) + (splitViewport.getBorderWidthPixels() + splitViewport.getBorderWidthBuffer()),
				0,
				splitViewport.getViewportWidth(OpenGLUtils::Viewport::RIGHT),
				splitViewport.getViewportHeight(OpenGLUtils::Viewport::RIGHT)
			);

			glfwSwapBuffers(window);
			glfwPollEvents(); // updates window state upon events like keyboard or mouse inputs;
		}

		nodeEditor.teardown();
		glfwDestroyWindow(window);
		glfwTerminate(); // clean up GLFW resources
	}
	catch (const char* str) {
		std::cerr << str << std::endl;
	}

	return 0;
}