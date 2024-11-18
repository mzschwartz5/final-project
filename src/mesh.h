#ifndef MESH_H
#define MESH_H

#include "shader.h"
#include "vertex.h"
#include <vector>
using std::vector;
#include <glad/gl.h>
#include <glm/glm.hpp>
using glm::mat4;

class Mesh {
public:
	Mesh(Shader shader);
	Mesh(vector<Vertex>&& vertices, vector<unsigned int>&& indices, Shader shader);
	uint addVertex(Vertex&& vertex);
	void addIndex(uint index);
	void setupMesh();
	void setDrawMode(GLenum mode) { drawMode = mode; }
	void draw(const mat4& viewMatrix, const mat4& projectionMatrix);

private:
	void bufferData();

	bool dirty = false;

	// Mesh data
	vector<Vertex> m_vertices;
	vector<unsigned int> m_indices;
	Shader m_shader;

	// Render data (OpenGL objects)
	unsigned int VAO, VBO, EBO;
	GLenum drawMode = GL_TRIANGLES;
};

#endif