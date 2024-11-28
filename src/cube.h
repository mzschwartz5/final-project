#ifndef CUBE_H
#define CUBE_H

#include "constants.h"
#include "mesh.h"
#include "vertex.h"
#include <vector>
using std::vector;

vector<Vertex> cubeVertices = {
	// Top
	Vertex(vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f,  0.0f, -1.0f), vec2(0.0f, 0.0f)),
	Vertex(vec3(0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(1.0f, 0.0f)),
	Vertex(vec3(0.5f,  0.5f, -0.5f), vec3(0.0f,  0.0f, -1.0f), vec2(1.0f, 1.0f)),
	Vertex(vec3(-0.5f,  0.5f, -0.5f), vec3(0.0f,  0.0f, -1.0f), vec2(0.0f, 1.0f)),

	// Bottom
	Vertex(vec3(-0.5f, -0.5f,  0.5f), vec3(0.0f,  0.0f,  1.0f), vec2(0.0f, 0.0f)),
	Vertex(vec3(0.5f, -0.5f,  0.5f), vec3(0.0f,  0.0f,  1.0f), vec2(1.0f, 0.0f)),
	Vertex(vec3(0.5f,  0.5f,  0.5f), vec3(0.0f,  0.0f,  1.0f), vec2(1.0f, 1.0f)),
	Vertex(vec3(-0.5f,  0.5f,  0.5f), vec3(0.0f,  0.0f,  1.0f), vec2(0.0f, 1.0f)),

	// Left
	Vertex(vec3(-0.5f,  0.5f,  0.5f), vec3(-1.0f,  0.0f,  0.0f), vec2(1.0f, 0.0f)),
	Vertex(vec3(-0.5f,  0.5f, -0.5f), vec3(-1.0f,  0.0f,  0.0f), vec2(1.0f, 1.0f)),
	Vertex(vec3(-0.5f, -0.5f, -0.5f), vec3(-1.0f,  0.0f,  0.0f), vec2(0.0f, 1.0f)),
	Vertex(vec3(-0.5f, -0.5f,  0.5f), vec3(-1.0f,  0.0f,  0.0f), vec2(0.0f, 0.0f)),

	// Right
	Vertex(vec3(0.5f,  0.5f,  0.5f), vec3(1.0f,  0.0f,  0.0f), vec2(1.0f, 0.0f)),
	Vertex(vec3(0.5f,  0.5f, -0.5f), vec3(1.0f,  0.0f,  0.0f), vec2(1.0f, 1.0f)),
	Vertex(vec3(0.5f, -0.5f, -0.5f), vec3(1.0f,  0.0f,  0.0f), vec2(0.0f, 1.0f)),
	Vertex(vec3(0.5f, -0.5f,  0.5f), vec3(1.0f,  0.0f,  0.0f), vec2(0.0f, 0.0f)),

	// Front
	Vertex(vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f, -1.0f,  0.0f), vec2(0.0f, 1.0f)),
	Vertex(vec3(0.5f, -0.5f, -0.5f), vec3(0.0f, -1.0f,  0.0f), vec2(1.0f, 1.0f)),
	Vertex(vec3(0.5f, -0.5f,  0.5f), vec3(0.0f, -1.0f,  0.0f), vec2(1.0f, 0.0f)),
	Vertex(vec3(-0.5f, -0.5f,  0.5f), vec3(0.0f, -1.0f,  0.0f), vec2(0.0f, 0.0f)),

	// Back
	Vertex(vec3(-0.5f,  0.5f, -0.5f), vec3(0.0f,  1.0f,  0.0f), vec2(0.0f, 1.0f)),
	Vertex(vec3(0.5f,  0.5f, -0.5f), vec3(0.0f,  1.0f,  0.0f), vec2(1.0f, 1.0f)),
	Vertex(vec3(0.5f,  0.5f,  0.5f), vec3(0.0f,  1.0f,  0.0f), vec2(1.0f, 0.0f)),
	Vertex(vec3(-0.5f,  0.5f,  0.5f), vec3(0.0f,  1.0f,  0.0f), vec2(0.0f, 0.0f)),
};

vector<unsigned int> cubeIndices = {
	0,  1,  2,      0,  2,  3,    // front
	4,  5,  6,      4,  6,  7,    // back
	8,  9,  10,     8,  10, 11,   // top
	12, 13, 14,     12, 14, 15,   // bottom
	16, 17, 18,     16, 18, 19,   // right
	20, 21, 22,     20, 22, 23,   // left
};

class Cube {
public:
    Cube(const vec3& position, const vec3& scale) 
		: m_mesh(std::move(cubeVertices), std::move(cubeIndices), Shader("../src/shaders/vertShader.vs", "../src/shaders/fragShader.fs"))
    {
		m_modelMatrix = glm::translate(m_modelMatrix, position);
		m_modelMatrix = glm::scale(m_modelMatrix, scale);
    }

    void draw(const mat4& viewMatrix, const mat4& projectionMatrix) {
		Shader& shader = m_mesh.getShader();
		shader.use();
		shader.setValue(Constants::VIEW_MATRIX, viewMatrix);
		shader.setValue(Constants::PROJECTION_MATRIX, projectionMatrix);
		shader.setValue(Constants::MODEL_MATRIX, m_modelMatrix);
		m_mesh.draw();
    }

private:
    Mesh m_mesh;
	mat4 m_modelMatrix = mat4(1.0f);
};

#endif