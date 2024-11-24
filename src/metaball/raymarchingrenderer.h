#ifndef RAYMARCHINGRENDERER_H
#define RAYMARCHINGRENDERER_H

#include "metaball.h"
#include <array>
#include <vector>
#include "quad.h"
using std::vector;
using std::array;
using glm::mat4;
using glm::vec3;

class RaymarchingRenderer {
public:
    RaymarchingRenderer(const vector<Metaball>& metaballs, const Quad& quad);
    ~RaymarchingRenderer();

    void render(const mat4& viewMatrix, const mat4& projectionMatrix);

private:
    static bool _instantiated;
    vector<Metaball> metaballs;
    unsigned int metaballPositionsBuffer;
    unsigned int metaballRadiiBuffer;
    unsigned int metaballScaleBuffer;
    Quad quad;
};

#endif // RAYMARCHINGRENDERER_H