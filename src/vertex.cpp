#include "vertex.h"

using glm::vec3;
using glm::vec2;

Vertex::Vertex(vec3 position, vec3 normal, vec2 texCoords) {
	m_position = position;
	m_normal = normal;
	m_texCoords = texCoords;
}