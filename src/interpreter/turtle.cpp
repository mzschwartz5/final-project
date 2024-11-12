#include "turtle.h"
bool Turtle::_instantiated = false;

void Turtle::rotate(float yaw, float pitch) {
    this->yaw += yaw;
    this->pitch += pitch;
}

void Turtle::move(float distance, bool draw) {
    vec3 newPos = position + vec3(
        distance * cos(yaw) * cos(pitch),
        distance * sin(yaw) * cos(pitch),
        distance * sin(pitch)
    );

    setPosition(newPos, draw);
}

void Turtle::setPosition(const vec3& pos, bool draw) {
    position = pos;

    if (!draw) return;

    uint vertIndex = mesh.addVertex(
        Vertex(
            pos,
            vec3(0.0f, 0.0f, 0.0f), // normal (hardcoded for now)
            vec2(0.0f, 0.0f)        // texture coordinates (hardcoded for now)
        )
    );

    mesh.addIndex(positionIndex);
    mesh.addIndex(vertIndex);

    positionIndex = vertIndex;
}

void Turtle::draw() {
    mesh.draw();
}