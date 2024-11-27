#include "turtle.h"
#include "../constants.h"
#include "../mathutils.h"
#include <array>

bool Turtle::_instantiated = false;

void Turtle::drawLines(const mat4& viewMatrix, const mat4& projectionMatrix) {
    Shader& shader = mesh.getShader();
    shader.use();
    shader.setValue(Constants::VIEW_MATRIX, viewMatrix);
    shader.setValue(Constants::PROJECTION_MATRIX, projectionMatrix);
    mesh.draw();
}

void Turtle::drawGeometry(
    const mat4& viewMatrix, 
    const vec2& viewportDims,
    float viewportOffset,
    const vec2& nearPlaneDims,
    float nearPlaneDistance
) {
    raymarchingRenderer.render(
        viewMatrix,
        viewportDims,
        viewportOffset,
        nearPlaneDims,
        nearPlaneDistance
    );
}

void Turtle::finalize() {
    raymarchingRenderer.setMetaballs(metaballs);
}

void Turtle::rotate(float yaw, float pitch) {
    this->yaw += yaw;
    this->pitch += pitch;
}

void Turtle::move(float distance, float scale, bool draw) {
    vec3 newPos = position + vec3(
        distance * cos(yaw) * cos(pitch),
        distance * sin(yaw) * cos(pitch),
        distance * sin(pitch)
    );

    setState(newPos, scale, draw);
}

void Turtle::move(float distance, bool draw) {
    move(distance, scale, draw);
}

void Turtle::setState(const vec3& newPos, float newScale, bool draw) {
    if (!draw) {
        position = newPos;
        scale = newScale;
        return;
    }

    // Add vertices for drawing turtle lines
    unsigned int vertIndex = mesh.addVertex(
        Vertex(
            newPos,
            vec3(0.0f, 0.0f, 0.0f), // normal (hardcoded for now)
            vec2(0.0f, 0.0f)        // texture coordinates (hardcoded for now)
        )
    );

    mesh.addIndex(positionIndex);
    mesh.addIndex(vertIndex);

    // Add metaballs for rendering 3D geometry
    std::array<vec3, METABALLS_PER_SEGMENT> resampledPositions 
        = MathUtils::linearResample<vec3, METABALLS_PER_SEGMENT>(position, newPos);

    std::array<vec3, METABALLS_PER_SEGMENT> resampledScales
        = MathUtils::linearResample<vec3, METABALLS_PER_SEGMENT>(vec3(scale), vec3(newScale));

    for (int i = 0; i < METABALLS_PER_SEGMENT; i++) {
        metaballs.emplace_back(Metaball{ resampledPositions[i], vec3(resampledScales[i]), 0.5f });
    }

    // Update turtle state
    positionIndex = vertIndex;
    position = newPos;
    scale = newScale;
}