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
	~Mesh();
	unsigned int addVertex(Vertex&& vertex);
	void addIndex(unsigned int index);
	void setupMesh();
	void setDrawMode(GLenum mode) { drawMode = mode; }
	unsigned int addTexture(const char* texturePath);
	void useTexture(unsigned int texture);
	unsigned int addSSBO(const void* data, size_t size);
	void updateSSBO(unsigned int SSBO, const void* data, size_t size);
	void draw();
	void reset();
	void clearSSBOs();
	void clearVertices();
	Shader& getShader() { return m_shader; }

private:
	void bufferData();

	bool dirty = false;

	// Mesh data
	vector<Vertex> m_vertices;
	vector<unsigned int> m_indices;
	Shader m_shader;

	// Render data (OpenGL objects)
	unsigned int VAO, VBO, EBO;
	vector<unsigned int> SSBOs;
	vector<unsigned int> textures;
	GLenum drawMode = GL_TRIANGLES;
};

#endif