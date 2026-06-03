#pragma once

#include <glm/glm.hpp>
#include <memory>

#include "graphics/Mesh.hpp"

class MeshFactory
{
public:
    static std::unique_ptr<Mesh> createSphere(unsigned int subdivisions,
                                              const glm::vec3 &color = glm::vec3(1.0f));

    static std::unique_ptr<Mesh> createCube(const glm::vec3 &color = glm::vec3(1.0f));
};
