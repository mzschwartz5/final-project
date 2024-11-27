#ifndef TURTLE_H
#define TURTLE_H

#include <glad/gl.h>
#include <glm/glm.hpp>
#include "mesh.h"
#include "../metaball/metaball.h"
#include "../metaball/raymarchingrenderer.h"
using glm::vec3;
using glm::mat4;
using std::vector;

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
    void move(float distance, float scale, bool draw = true);
    void rotate(float yaw, float pitch);
    void setState(const vec3& pos, float scale = 1.0f, bool draw = true);
    const vec3& getPosition() const { return position; }
    void setRotation(float yaw, float pitch) { this->yaw = yaw; this->pitch = pitch; }
    float getYaw() const { return yaw; }
    float getPitch() const { return pitch; }
    unsigned int getPositionIndex() const { return positionIndex; }
    void setPositionIndex(unsigned int index) { positionIndex = index; }
    float getScale() const { return scale; }

    void finalize();
    void drawLines(const mat4& viewMatrix, const mat4& projectionMatrix);
    void drawGeometry(
        const mat4& viewMatrix, 
        const vec2& viewportDims,
        float viewportOffset,
        const vec2& nearPlaneDims,
        float nearPlaneDistance
    );

private:
    static bool _instantiated;
    static constexpr int METABALLS_PER_SEGMENT = 10;
    
    RaymarchingRenderer raymarchingRenderer;
    vec3 position = vec3(0.0f, 0.0f, 0.0f);
    float scale = 1.0f;
    float pitch = 0.0f; // radians
    float yaw = 0.0f;   // radians
    unsigned int positionIndex;
    Mesh mesh;
    // Geometry is made of metaballs
    vector<Metaball> metaballs;
};
#endif