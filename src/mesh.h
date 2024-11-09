#ifndef MESH_H
#define MESH_H

#include "shader.h"
#include "vertex.h"
#include <vector>
using std::vector;

class Mesh {
public:
	Mesh(vector<Vertex>&& vertices, vector<unsigned int>&& indices, Shader shader);
	void Draw() const;
private:
	// Mesh data
	vector<Vertex> m_vertices;
	vector<unsigned int> m_indices;
	Shader m_shader;

	// Render data (OpenGL objects)
	unsigned int VAO, VBO, EBO;
	void setupMesh();
};

#endif