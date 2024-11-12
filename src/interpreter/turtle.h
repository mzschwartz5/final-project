#ifndef TURTLE_H
#define TURTLE_H

#include <glad/gl.h>
#include <glm/glm.hpp>
using glm::vec3;
#include "mesh.h"

// This turtle draws the skeleton of the geometry.
class Turtle {
public:
    // Temporarily hardcode shader - just for visualizing the lines drawn by the turtle
    // Honestly, eventually, we won't even draw anything here. This is just for visualizing the skeleton.
    Turtle() : mesh(Shader("../src/shaders/vertShader.vs", "../src/shaders/fragShader.fs")) {
        assert(!_instantiated);
        _instantiated = true;
        mesh.setDrawMode(GL_LINES);
        positionIndex = mesh.addVertex(Vertex(vec3(0.0f, 0.0f, 0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f)));
    }

    ~Turtle() {
        _instantiated = false;
    }

    void move(float distance, bool draw = true);
    void rotate(float yaw, float pitch);
    const vec3& getPosition() const { return position; }
    void draw();

private:
    void setPosition(const vec3& pos, bool draw = true);
    static bool _instantiated;
    vec3 position = vec3(0.0f, 0.0f, 0.0f);
    float pitch = 0.0f; // radians
    float yaw = 0.0f;   // radians
    unsigned int positionIndex;
    Mesh mesh;
};
#endif