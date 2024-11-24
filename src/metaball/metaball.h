#ifndef METABALL_H
#define METABALL_H
#include <glm/glm.hpp>
using glm::vec3;


class Metaball {
public:
    Metaball(const glm::vec3& center, const glm::vec3& scale, float radius) : center(center), scale(scale), radius(radius) {};
    ~Metaball() {};

    float getRadius() const { return radius; }
    const vec3& getScale() const { return scale; }
    const vec3& getCenter() const { return center; }

    // Evaluate the metaball field (an inverse square field) at a given point
    float evaluate(const vec3& point) const {
        // Rather than scaling the metaball (multiplication), we scale the input point (division)
        const vec3 difference = (point / scale) - center;
        return radius * radius / (difference.x * difference.x + difference.y * difference.y + difference.z * difference.z);
    }

    vec3 gradient(const vec3& point) const {
        const vec3 difference = (point / scale) - center;
        float squaredDistance = difference.x * difference.x + difference.y * difference.y + difference.z * difference.z;
        return -2.0f * radius * radius * difference / (scale * squaredDistance * squaredDistance);
    }

private:
    vec3 center;
    vec3 scale;
    float radius;
};

#endif