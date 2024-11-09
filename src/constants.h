#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
using glm::dvec3;

namespace Constants {
	inline constexpr float SCR_WIDTH{ 800.0 };
	inline constexpr float SCR_HEIGHT{ 600.0 };
	inline constexpr dvec3 ORIGIN{0.0, 0.0, 0.0};
}

#endif