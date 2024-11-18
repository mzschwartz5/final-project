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
#include <vector>
#include "metaball/metaball.h"
#include "metaball/metaballrenderer.h"

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

		std::vector<Metaball> metaballs;
		metaballs.push_back(Metaball{ glm::vec3(0.0), glm::vec3{1.0f}, 1.0f });

		// MetaballRenderer metaballRenderer(metaballs);

		// Main render loop
		// Draw images until told to explicitly stop (e.g. x out of window)
		while (!glfwWindowShouldClose(window))
		{
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			// Render left viewport
			splitViewport.setViewport(OpenGLUtils::Viewport::LEFT);
			turtle.draw(cameraLeft.calcViewMatrix(), cameraLeft.calcProjectionMatrix(splitViewport.getViewportWidth(OpenGLUtils::Viewport::LEFT), splitViewport.getViewportHeight(OpenGLUtils::Viewport::LEFT)));
			// Render right viewport
			splitViewport.setViewport(OpenGLUtils::Viewport::RIGHT);
			// metaballRenderer.render(cameraRight.calcViewMatrix(), cameraRight.calcProjectionMatrix(splitViewport.getViewportWidth(OpenGLUtils::Viewport::RIGHT), splitViewport.getViewportHeight(OpenGLUtils::Viewport::RIGHT)));

			// Render a border bar between the two viewports
			splitViewport.setViewport(OpenGLUtils::Viewport::BORDER);
			splitViewport.drawViewportBorder();

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