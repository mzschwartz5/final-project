#include "mesh.h"
#include <glad/gl.h> 
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "camera.h"
#include "constants.h"

Mesh::Mesh(vector<Vertex>&& vertices, vector<unsigned int>&& indices, Shader shader)
	: m_vertices(std::move(vertices)), m_indices(std::move(indices)), m_shader(shader) {

	setupMesh();
}

void Mesh::Draw() {
	m_shader.use();
	// TODO: getting the camera matrices probably shouldn't belong here. Or at least, it should be cached in the Camera class.
	m_shader.setValue(Constants::VIEW_MATRIX, Camera::getInstance().calcViewMatrix());
	m_shader.setValue(Constants::PROJECTION_MATRIX, Camera::getInstance().calcProjectionMatrix());
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0); // DRAW
	glBindVertexArray(0); // unbind
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // unbind
}

void Mesh::setupMesh() {
	// Generate buffer object identifiers
	glGenVertexArrays(1, &VAO); // (VAO stores VBO and EBO along with metadata about each)
	glGenBuffers(1, &VBO);		// (VBO stores actual vertex positions in NDC space)
	glGenBuffers(1, &EBO);		// (EBO stores vertex indices)

	// Bind to OpenGL state so we can use them
	glBindVertexArray(VAO); 
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	// Set data to now-bound arrays
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), m_vertices.data(), GL_STATIC_DRAW); // sets to VBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), m_indices.data(), GL_STATIC_DRAW); // sets to EBO

	// Dictate how to interpret vertex data (which attributes are where, in the array)
	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// Vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_normal));
	// Vertex texture coordinates
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_texCoords));

	// Unbind VAO until use
	glBindVertexArray(0);
}	