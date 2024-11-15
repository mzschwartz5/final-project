#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
using glm::dvec3;
#include<string>
using std::string;

namespace Constants {
	inline constexpr unsigned int SCR_WIDTH{ 1066 };
	inline constexpr unsigned int SCR_HEIGHT{ 600 };
	inline constexpr dvec3 ORIGIN{0.0, 0.0, 0.0};
	inline const string VIEW_MATRIX = "view";
	inline const string PROJECTION_MATRIX = "projection";
}

#endif