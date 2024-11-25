#include "raymarchingrenderer.h"

bool RaymarchingRenderer::_instantiated = false;

RaymarchingRenderer::RaymarchingRenderer(const vector<Metaball>& metaballs, Quad&& quad) :
    metaballs(metaballs),
    quad(std::move(quad)) {

    assert(!_instantiated);
    _instantiated = true;

    // TODO: this has to be more dynamic, so that we can add and remove metaballs
    // Note: SSBOs are 16-byte aligned, so we have to pad the vec3s to vec4s
    vector<vec4> positions;
    vector<vec4> scales;
    vector<float> radii;

    for (const Metaball& metaball : metaballs) {
        positions.push_back(vec4(metaball.getCenter(), 0.0f));
        scales.push_back(vec4(metaball.getScale(), 0.0f)); // TODO: can pack this with radius by just have 3 radii instead of scales and radius
        radii.push_back(metaball.getRadius()); 
    }

    Mesh& quadMesh = this->quad.getMesh();
    metaballPositionsBuffer = quadMesh.addSSBO(positions.data(), sizeof(vec4) * metaballs.size());
    metaballScaleBuffer = quadMesh.addSSBO(scales.data(), sizeof(vec4) * metaballs.size());
    metaballRadiiBuffer = quadMesh.addSSBO(radii.data(), sizeof(float) * metaballs.size());
}

RaymarchingRenderer::~RaymarchingRenderer() {
    _instantiated = false;
}


void RaymarchingRenderer::render(
    const mat4& viewMatrix,
    const vec2& viewportDims,
    float viewportOffset,
    const vec2& nearPlaneDims,
    float nearPlaneDistance
) {

    Shader& shader = quad.getMesh().getShader();
    shader.use();
    shader.setValue("isoValue", ISOVALUE);
    shader.setValue("invViewMatrix", glm::inverse(viewMatrix));
    shader.setValue("viewportDims", viewportDims);
    shader.setValue("viewportOffset", viewportOffset);
    shader.setValue("nearPlaneDims", nearPlaneDims);
    shader.setValue("nearPlaneDist", nearPlaneDistance);
    shader.setValue("numMetaballs", static_cast<int>(metaballs.size()));

    quad.draw();
}