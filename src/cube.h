#ifndef CUBE_H
#define CUBE_H

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
    Cube() : m_mesh(std::move(cubeVertices), std::move(cubeIndices), Shader("../src/shaders/vertShader.vs", "../src/shaders/fragShader.fs"))
    {
    }

    void draw() const {
        m_mesh.Draw();
    }

private:
    Mesh m_mesh;

};

#endif