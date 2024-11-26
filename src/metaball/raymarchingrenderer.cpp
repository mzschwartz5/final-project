#include "raymarchingrenderer.h"

bool RaymarchingRenderer::_instantiated = false;

RaymarchingRenderer::RaymarchingRenderer() : quad(Shader("../src/shaders/fullscreen.vs", "../src/shaders/raymarch.fs")) {
    assert(!_instantiated);
    _instantiated = true;
}

RaymarchingRenderer::~RaymarchingRenderer() {
    _instantiated = false;
}

void RaymarchingRenderer::setMetaballs(const vector<Metaball>& metaballs) {
    numMetaballs = metaballs.size();

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

    // TODO: should use updateSSBO if already created
    Mesh& quadMesh = quad.getMesh();
    quadMesh.addSSBO(positions.data(), sizeof(vec4) * metaballs.size());
    quadMesh.addSSBO(scales.data(), sizeof(vec4) * metaballs.size());
    quadMesh.addSSBO(radii.data(), sizeof(float) * metaballs.size());
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
    shader.setValue("invViewMatrix", glm::inverse(viewMatrix));
    shader.setValue("viewportDims", viewportDims);
    shader.setValue("viewportOffset", viewportOffset);
    shader.setValue("nearPlaneDims", nearPlaneDims);
    shader.setValue("nearPlaneDist", nearPlaneDistance);
    shader.setValue("numMetaballs", numMetaballs);

    quad.draw();
}