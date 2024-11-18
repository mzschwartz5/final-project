#version 330 core

in vec3 Normal;
out vec4 FragColor;

void main()
{
	// Simple diffuse lighting
    vec3 lightDir = normalize(vec3(0.0, 0.0, -1.0));
    float diff = max(dot(Normal, lightDir), 0.0);
    FragColor = vec4(diff, diff, diff, 1.0);
}