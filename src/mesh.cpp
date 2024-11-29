#include "mesh.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "camera.h"
#include "constants.h"
#include "stb_image.h"

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

	for (unsigned int texture : textures) {
		glDeleteTextures(1, &texture);
	}
}

/* For now, pass in matrices directly. In future, consider accessing differently */
void Mesh::draw() {
	if (dirty) {
		bufferData();
		dirty = false;
	}

	for (unsigned int i = 0; i < SSBOs.size(); i++) {
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, i, SSBOs[i]);
	}

	glBindVertexArray(VAO);
	glDrawElements(drawMode, m_indices.size(), GL_UNSIGNED_INT, 0); // DRAW
	glBindVertexArray(0); // unbind
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // unbind

}

unsigned int Mesh::addTexture(const char* texturePath) {
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // x-axis
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // y-axis
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
	if (data) {
		GLenum format;
		if (nrChannels == 1) {
			format = GL_RED;
		}
		else if (nrChannels == 3) {
			format = GL_RGB;
		}
		else if (nrChannels == 4) {
			format = GL_RGBA;
		}
		else {
			throw "Unsupported number of channels in texture";
		}

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		throw "Failed to load texture";
	}
	stbi_image_free(data);

	textures.push_back(texture);
	return texture;
}

void Mesh::useTexture(unsigned int texture) {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
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

void Mesh::reset() {
	m_vertices.clear();
	m_indices.clear();
	for (unsigned int SSBO : SSBOs) {
		glDeleteBuffers(1, &SSBO);
	}
	SSBOs.clear();
	dirty = true;
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