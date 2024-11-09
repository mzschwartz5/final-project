#ifndef VERTEX_H
#define VERTEX_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using glm::vec3;
using glm::vec2;

class Vertex {
public:
	Vertex(vec3 position, vec3 normal, vec2 texCoords);
	// While I would prefer the following 3 variables to be private, I need to be able to use the offsetof macro on them 
	// for an OpenGL call, and this isn't possible on private variables.
	vec3 m_position;
	vec3 m_normal;
	vec2 m_texCoords;
};

#endif // !VERTEX_H