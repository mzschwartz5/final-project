#include "metaballrenderer.h"
#include "../vertex.h"

bool MetaballRenderer::_instantiated = false;

void MetaballRenderer::createMesh() {
    vec3 minBounds {std::numeric_limits<float>::max()};
    vec3 maxBounds {std::numeric_limits<float>::min()};

    for (const Metaball& metaball : metaballs) {
        minBounds = glm::min(minBounds, metaball.getCenter() - (metaball.getRadius() * metaball.getScale()));
        maxBounds = glm::max(maxBounds, metaball.getCenter() + (metaball.getRadius() * metaball.getScale()));
    }

    for (float x = minBounds.x; x < maxBounds.x; x += gridResolution.x) {
        for (float y = minBounds.y; y < maxBounds.y; y += gridResolution.y) {
            for (float z = minBounds.z; z < maxBounds.z; z += gridResolution.z) {
                marchingCubesSingleCell(vec3(x, y, z));
            }
        }
    }
}

void MetaballRenderer::marchingCubesSingleCell(const glm::vec3& cellCorner) {
    array<float, 8> fieldValues;
    array<vec3, 8> gradientValues;
    int caseIndex = 0;
    for (int i = 0; i < 8; i++) {
        glm::vec3 vertexPosition = cellCorner + glm::vec3(VERTICES[i][0], VERTICES[i][1], VERTICES[i][2]) * gridResolution;
        fieldValues[i] = metaballField(vertexPosition);
        gradientValues[i] = metaballGradient(vertexPosition);

        if (fieldValues[i] < isoLevel) continue;
        caseIndex += (1 << i);
    }

    const vector<array<int, 3>>& faces = CASES[caseIndex];

    for (const array<int, 3>& face : faces) {
        int newVertIndex = -1;

        for (int i = 0; i < 3; ++i) {
            const array<int, 2>& edge = EDGES[face[i]];
            const array<int, 3>& vertexA = VERTICES[edge[0]];
            const array<int, 3>& vertexB = VERTICES[edge[1]];
            float fieldA = fieldValues[edge[0]];
            float fieldB = fieldValues[edge[1]];

            // Interpolate between the two vertices to find the point on the edge where the field value is equal to the iso level
            // (Assumes the field varies linearly between the two vertices)
            float t0 = (isoLevel - fieldA) / (fieldB - fieldA);
            float t1 = 1.0f - t0;
            const vec3 newVert = cellCorner
                            + (vec3(vertexA[0], vertexA[1], vertexA[2]) * t0
                            + vec3(vertexB[0], vertexB[1], vertexB[2]) * t1) * gridResolution;

            const vec3 newNormal = glm::normalize(
                gradientValues[edge[0]] * t0 + gradientValues[edge[1]] * t1
            );

            // No attempt made to remove duplicate vertices; this is a simple implementation, a proof of concept.
            newVertIndex =  mesh.addVertex(Vertex(newVert, newNormal, vec3(0.0f)));
        }

        mesh.addIndex(newVertIndex - 2);
        mesh.addIndex(newVertIndex - 1);
        mesh.addIndex(newVertIndex);
    }
}

/*
    For now, simply iterate over all metaballs and sum their field contributions.
    In the future, could accelerate this with a hierarchical spatial structure, or
    use the properties of the character skeleton to dictate which metaballs influence each other.
*/
float MetaballRenderer::metaballField(const glm::vec3& point) {
    float field = 0.0f;
    for (const Metaball& metaball : metaballs) {
        field += metaball.evaluate(point);
    }
    return field;
}

vec3 MetaballRenderer::metaballGradient(const vec3& point) {
    vec3 gradient = vec3(0.0f);
    for (const Metaball& metaball : metaballs) {
        gradient += metaball.gradient(point);
    }
    return gradient;
}