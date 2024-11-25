#ifndef RAYMARCHINGRENDERER_H
#define RAYMARCHINGRENDERER_H

#include "metaball.h"
#include <array>
#include <vector>
#include "quad.h"
using std::vector;
using std::array;
using glm::mat4;
using glm::vec4;
using glm::vec3;
using glm::vec2;

class RaymarchingRenderer {
public:
    RaymarchingRenderer(const vector<Metaball>& metaballs, Quad&& quad);
    ~RaymarchingRenderer();

    void render(
        const mat4& viewMatrix, 
        const vec2& viewportDims,
        float viewportOffset,
        const vec2& nearPlaneDims,
        float nearPlaneDistance
    );

private:
    static bool _instantiated;
    static constexpr float ISOVALUE = 0.05f;
    vector<Metaball> metaballs;
    unsigned int metaballPositionsBuffer;
    unsigned int metaballRadiiBuffer;
    unsigned int metaballScaleBuffer;
    Quad quad;
};

#endif // RAYMARCHINGRENDERER_H