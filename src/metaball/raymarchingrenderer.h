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
    RaymarchingRenderer();
    ~RaymarchingRenderer();

    void setMetaballs(const vector<Metaball>& metaballs);

    void render(
        const mat4& viewMatrix, 
        const vec2& viewportDims,
        float viewportOffset,
        const vec2& nearPlaneDims,
        float nearPlaneDistance
    );

private:
    static bool _instantiated;
    unsigned int metaballPositionsBuffer;
    unsigned int metaballRadiiBuffer;
    unsigned int metaballScaleBuffer;
    unsigned int numMetaballs = 0;
    Quad quad;
};

#endif // RAYMARCHINGRENDERER_H