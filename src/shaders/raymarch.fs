#version 430 core


uniform mat4 invViewMatrix;
uniform vec2 viewportDims;
uniform float viewportOffset;
uniform vec2 nearPlaneDims;
uniform float nearPlaneDist;
uniform uint numMetaballs;

out vec4 FragColor;

// SSBOs have to be 16-byte aligned! (So even though these are really vec3s, we use vec4s)
layout(std430, binding = 0) buffer MetaballPositions {
    vec4 mbPositions[];
};

layout(std430, binding = 1) buffer MetaballScales {
    vec4 mbScales[];
};

layout(std430, binding = 2) buffer MetaballRadii {
    float mbRadii[];
};

const vec3 lightDir = normalize(vec3(1.0f, 1.0f, 1.0f));
const int MAX_STEPS = 50;
const float MAX_STEP_SIZE = 1.0f;
const float MIN_STEP_SIZE = 0.05f;
const float EPISILON = 0.001f;
const float K = 0.25f;

float evaluateMetaballField(vec3 point, vec3 center, vec3 scale, float radius) {
    point /= scale;
    return length(point - center) - radius;
}

vec3 gradientMetaballField(vec3 point, vec3 center, vec3 scale, float radius) {
    point /= scale;
    // Gradient of the metaball field is the normalized vector from the point to the center
    return (point - center) / radius;
}

// Quadratic smooth minimum function: IQ (https://iquilezles.org/articles/smin/)
float smoothMin( float a, float b, float k)
{
    k *= 4.0;
    float x = (b-a)/k;
    float g = (x> 1.0) ? x :
              (x<-1.0) ? 0.0 :
              (x*(2.0+x)+1.0)/4.0;
    return b - k * g;
}

// Also from IQ:
vec4 smoothMinGradient (vec4 a, vec4 b, float k)
{
    float h = 0.5 - min(abs(a.x-b.x)/(8.0*k), 0.5);
    float s = h*h*k;
    return (a.x<b.x) ? vec4(a.x-s, mix(a.yzw,b.yzw,h)):
                       vec4(b.x-s, mix(b.yzw,a.yzw,h));
}

vec3 rayPosition(in vec3 rayOrigin, in vec3 rayDirection, in float t) {
    return (rayOrigin + (rayDirection * t));
}

vec2 scaleFragCoord(in vec2 fragCoord) {
    // Account for the split viewport (fragCoord is relative to window, not viewport)
    fragCoord -= vec2(viewportOffset, 0.0f);
    // Normalize to [0, 1]
    vec2 scaledCoord = (fragCoord / viewportDims);
    // Scale to [-0.5, 0.5]
    scaledCoord -= 0.5f;
    // Scale to near plane dimensions [-nearPlaneDims/2, nearPlaneDims/2]
    scaledCoord *= (nearPlaneDims);
    return scaledCoord;
}

float raymarch(in vec3 rayOrigin, in vec3 rayDirection, out float field) {
    float t = 0.0f;
    float lastT = 0.0f;
    float stepSize = 0.1f;
    float lastField = 0.0f;

    for (int i = 0; i < MAX_STEPS; ++i) {
        vec3 p = rayPosition(rayOrigin, rayDirection, t);
        field = evaluateMetaballField(p, mbPositions[0].xyz, mbScales[0].xyz, mbRadii[0]);

        for (int j = 1; j < numMetaballs; j++) {
            field = smoothMin(field, evaluateMetaballField(p, mbPositions[j].xyz, mbScales[j].xyz, mbRadii[j]), K);
        }

        if (field <= EPISILON) {
            // Interpolate between the last and current t values to get a more accurate intersection point
            return mix(lastT, t,  -lastField / (field - lastField));
        }

        lastT = t;
        lastField = field;
        t += field; // Sphere tracing
    }
    return -1.0f;
}

vec3 gradientAtPoint(in vec3 point) {
    vec3 gradient = gradientMetaballField(point, mbPositions[0].xyz, mbScales[0].xyz, mbRadii[0]);
    float field = evaluateMetaballField(point, mbPositions[0].xyz, mbScales[0].xyz, mbRadii[0]);

    for (int i = 1; i < numMetaballs; i++) {
        vec3 newGradient = gradientMetaballField(point, mbPositions[i].xyz, mbScales[i].xyz, mbRadii[i]);
        float newField = evaluateMetaballField(point, mbPositions[i].xyz, mbScales[i].xyz, mbRadii[i]);
        vec4 gradAndField = smoothMinGradient(vec4(field, gradient), vec4(newField, newGradient), K);
        gradient = gradAndField.yzw;
        field = gradAndField.x;
    }

    return normalize(gradient);
}

void main()
{

    vec2 fragCoord = gl_FragCoord.xy;
    vec2 scaledFragCoord = scaleFragCoord(fragCoord);
    vec3 rayDirectionCamSpace = vec3(scaledFragCoord, -nearPlaneDist);
    vec3 rayDirection = normalize((invViewMatrix * vec4(rayDirectionCamSpace, 0.0f)).xyz); // world space
    vec3 cameraPos = invViewMatrix[3].xyz;

    float field;
    float t = raymarch(cameraPos, rayDirection, field);
    if (t < 0.0f) {
        FragColor = vec4(0.2f, 0.3f, 0.3f, 1.0);
        return;
    }

    vec3 p = rayPosition(cameraPos, rayDirection, t);
    vec3 normal = gradientAtPoint(p);

    // Simple diffuse lighting
    float diff = max(dot(normal, lightDir), 0.0);
    FragColor = vec4(diff, diff, diff, 1.0);
}