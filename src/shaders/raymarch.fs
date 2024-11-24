#version 330 core
out vec4 FragColor;

layout(std430, binding = 0) buffer MetaballPositions {
    vec3 positions[];
};

layout(std430, binding = 1) buffer MetaballScales {
    vec3 scales[];
};

layout(std430, binding = 2) buffer MetaballRadii {
    float radii[];
};

void main()
{
	vec3 position = positions[0];
	FragColor = vec4(position.xyz, 1.0f);
}