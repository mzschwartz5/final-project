#ifndef QUAD_H
#define QUAD_H

#include "shader.h"
#include "mesh.h"
#include "vertex.h"

class Quad {
public:
    Quad(const Shader& shader) :
        shader(shader),
        mesh(
            std::vector<Vertex>{
                Vertex(glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)),
                Vertex(glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)),
                Vertex(glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f)),
                Vertex(glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f))
            },
            std::vector<unsigned int>{
                0, 1, 2,
                0, 2, 3
            },
            shader
        )
        {
            quadTexture = mesh.addTexture("../textures/red_wax.png");
        }

    void draw() {
        mesh.useTexture(quadTexture);
        mesh.draw();
    }

    Mesh& getMesh() {
        return mesh;
    }

private:
    Shader shader;
    Mesh mesh;
    unsigned int quadTexture;
};

#endif // QUAD_H