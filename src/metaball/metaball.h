#ifndef METABALL_H
#define METABALL_H
#include <glm/glm.hpp>
using glm::vec3;


class Metaball {
public:
    Metaball(const glm::vec3& center, const glm::vec3& scale, float radius) : center(center), scale(scale), radius(radius) {};
    ~Metaball() {};

    float getRadius() const { return radius; }
    float getScale() const { return scale.x; }
    const vec3& getCenter() const { return center; }

    // Evaluate the metaball field (an inverse square field) at a given point
    float evaluate(const vec3& point) const {
        // Rather than scaling the metaball (multiplication), we scale the input point (division)
        const vec3 difference = (point / scale) - center;
        return radius / (difference.x * difference.x + difference.y * difference.y + difference.z * difference.z);
    }
private:
    vec3 center;
    vec3 scale;
    float radius;
};

#endif