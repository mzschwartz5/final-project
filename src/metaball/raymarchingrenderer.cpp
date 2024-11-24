#include "raymarchingrenderer.h"

bool RaymarchingRenderer::_instantiated = false;

RaymarchingRenderer::RaymarchingRenderer(const vector<Metaball>& metaballs, const Quad& quad) :
    metaballs(metaballs),
    quad(quad) {

    assert(!_instantiated);
    _instantiated = true;

    // TODO: this has to be more dynamic, so that we can add and remove metaballs
    vector<vec3> positions;
    vector<vec3> scales;
    vector<float> radii;

    for (const Metaball& metaball : metaballs) {
        positions.push_back(metaball.getCenter());
        scales.push_back(metaball.getScale());
        radii.push_back(metaball.getRadius());
    }

    Mesh& quadMesh = this->quad.getMesh();
    metaballPositionsBuffer = quadMesh.addSSBO(&positions, sizeof(vec3) * metaballs.size());
    metaballScaleBuffer = quadMesh.addSSBO(&scales, sizeof(vec3) * metaballs.size());
    metaballRadiiBuffer = quadMesh.addSSBO(&radii, sizeof(float) * metaballs.size());
}

RaymarchingRenderer::~RaymarchingRenderer() {
    _instantiated = false;
}


void RaymarchingRenderer::render(const mat4& viewMatrix, const mat4& projectionMatrix) {
    quad.draw(viewMatrix, projectionMatrix);
}