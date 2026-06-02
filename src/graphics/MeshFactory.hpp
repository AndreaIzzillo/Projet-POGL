#pragma once

#include <glm/glm.hpp>
#include <memory>

#include "graphics/Mesh.hpp"

class MeshFactory
{
public:
    static std::unique_ptr<Mesh> createSphere(float radius = 1.0f, unsigned int sectors = 64,
                                              unsigned int stacks = 32,
                                              const glm::vec3 &color = glm::vec3(1.0f));

    // Debug meshes
    static std::unique_ptr<Mesh> createTriangle();
    static std::unique_ptr<Mesh> createCube();
};