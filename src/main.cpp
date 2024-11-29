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
#include "openglutils.h"
#include "cube.h"
#include "ui/nodeeditor.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imnodes.h"

int main() {
    // Top level error handling
	try {
	    glfwSetErrorCallback(OpenGLUtils::glfwErrorCallback);

		// GLFW initialization and global settings
		GLFWwindow* window = OpenGLUtils::initializeGLFW();
		Camera cameraLeft;
		Camera cameraRight;
		OpenGLUtils::SplitViewport splitViewport(&cameraLeft, &cameraRight);
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
			}

			nodeEditor.reset();
			
			glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			// Render left viewport
			splitViewport.setViewport(OpenGLUtils::Viewport::LEFT);
			turtle.drawLines(cameraLeft.calcViewMatrix(), cameraLeft.calcProjectionMatrix(splitViewport.getViewportWidth(OpenGLUtils::Viewport::LEFT), splitViewport.getViewportHeight(OpenGLUtils::Viewport::LEFT)));

			// Render right viewport
			splitViewport.setViewport(OpenGLUtils::Viewport::RIGHT);
			// turtle.drawGeometry(
			// 	cameraRight.calcViewMatrix(),
			// 	glm::vec2(splitViewport.getViewportWidth(OpenGLUtils::Viewport::RIGHT), splitViewport.getViewportHeight(OpenGLUtils::Viewport::RIGHT)),
			// 	splitViewport.getViewportWidth(OpenGLUtils::Viewport::LEFT),
			// 	cameraRight.getNearPlaneDims(),
			// 	cameraRight.getNearPlane()
			// );

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