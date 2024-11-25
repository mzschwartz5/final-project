#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
	unsigned int ID;
	Shader(const char* vertexPath, const char* fragmentPath);
	void use() const;
	void setValue(const std::string&, bool) const;
	void setValue(const std::string&, int) const;
	void setValue(const std::string&, float) const;
	void setValue(const std::string&, const glm::mat4&) const;
	void setValue(const std::string&, const glm::vec2&) const;

private:
	void checkCompileErrors(unsigned int shader, std::string type);
};

#endif