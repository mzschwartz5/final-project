#version 430 core


uniform float isoValue;
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

const vec3 lightDir = normalize(vec3(-1.0f, -1.0f, -1.0f));
const int MAX_STEPS = 100;
const float MAX_STEP_SIZE = 1.0f;
const float MIN_STEP_SIZE = 0.05f;

float evaluateMetaballField(vec3 point, vec3 center, vec3 scale, float radius) {
    vec3 difference = (point / scale) - center;
    return radius * radius / dot(difference, difference);
}

vec3 gradientMetaballField(vec3 point, vec3 center, vec3 scale, float radius) {
    vec3 difference = (point / scale) - center;
    float squaredDistance = dot(difference, difference);
    return -2.0f * radius * radius * difference / (scale * squaredDistance * squaredDistance);
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
        field = 0.0f;

        for (int j = 0; j < numMetaballs; j++) {
            field += evaluateMetaballField(p, mbPositions[j].xyz, mbScales[j].xyz, mbRadii[j]);
        }

        if (field > isoValue) {
            // Interpolate between the last and current t values to get a more accurate intersection point
            return mix(lastT, t, (isoValue - lastField) / (field - lastField));
        }

        // Adaptive step size based on the difference between fields at the current and last step
        // normalized to the isovalue. Small difference -> take larger step, large difference -> take smaller step
        float normFieldDiff = (field - lastField) / isoValue;
        if (normFieldDiff <= 0.01f) 
        {
            stepSize = min(stepSize * 2.0f, MAX_STEP_SIZE); 
        }
        else {
            stepSize = max(stepSize * (1.0f - normFieldDiff), MIN_STEP_SIZE);
        }

        lastT = t;
        lastField = field;
        t += stepSize;
    }
    return -1.0f;
}

vec3 gradientAtPoint(in vec3 point) {
    vec3 gradient = vec3(0.0f);
    for (int i = 0; i < numMetaballs; i++) {
        gradient += gradientMetaballField(point, mbPositions[i].xyz, mbScales[i].xyz, mbRadii[i]);
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