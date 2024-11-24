#include "mesh.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "camera.h"
#include "constants.h"

Mesh::Mesh(vector<Vertex>&& vertices, vector<unsigned int>&& indices, Shader shader)
	: m_vertices(std::move(vertices)), m_indices(std::move(indices)), m_shader(shader) {

	setupMesh();
	bufferData();
}

Mesh::Mesh(Shader shader) : m_shader(shader) {
	setupMesh();
}

Mesh::~Mesh() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	for (unsigned int SSBO : SSBOs) {
		glDeleteBuffers(1, &SSBO);
	}
}

/* For now, pass in matrices directly. In future, consider accessing differently */
void Mesh::draw(const mat4& viewMatrix, const mat4& projectionMatrix) {
	if (dirty) {
		bufferData();
		dirty = false;
	}

	m_shader.use();
	m_shader.setValue(Constants::VIEW_MATRIX, viewMatrix);
	m_shader.setValue(Constants::PROJECTION_MATRIX, projectionMatrix);

	for (unsigned int SSBO : SSBOs) {
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, SSBO, SSBO);
	}

	glBindVertexArray(VAO);
	glDrawElements(drawMode, m_indices.size(), GL_UNSIGNED_INT, 0); // DRAW
	glBindVertexArray(0); // unbind
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // unbind

}

unsigned int Mesh::addSSBO(const void* data, size_t size) {
	unsigned int SSBO;
	glGenBuffers(1, &SSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, SSBOs.size(), SSBO);
	SSBOs.push_back(SSBO);
	return SSBO;
}

void Mesh::updateSSBO(unsigned int SSBO, const void* data, size_t size) {
	// TODO: use glBufferSubData when possible
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_STATIC_DRAW);
}

unsigned int Mesh::addVertex(Vertex&& vertex) {
	m_vertices.push_back(vertex);
	dirty = true;
	return m_vertices.size() - 1;
}

void Mesh::addIndex(unsigned int index) {
	m_indices.push_back(index);
	dirty = true;
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

void Mesh::bufferData() {
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), m_vertices.data(), GL_STATIC_DRAW); // sets to VBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), m_indices.data(), GL_STATIC_DRAW); // sets to EBO
}
