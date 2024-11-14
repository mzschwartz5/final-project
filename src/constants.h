#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
using glm::dvec3;
#include<string>
using std::string;

namespace Constants {
	inline constexpr float SCR_WIDTH{ 800.0 };
	inline constexpr float SCR_HEIGHT{ 600.0 };
	inline constexpr dvec3 ORIGIN{0.0, 0.0, 0.0};
	inline constexpr float VIEWPORT_BORDER_WIDTH{ 4.0f };
	inline const string VIEW_MATRIX = "view";
	inline const string PROJECTION_MATRIX = "projection";
}

#endif