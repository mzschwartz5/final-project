#include "turtle.h"
bool Turtle::_instantiated = false;

void Turtle::setPosition(const vec3& pos, bool draw) {
    if (!draw) return;

    position = pos;
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