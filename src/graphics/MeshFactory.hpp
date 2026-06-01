#pragma once

#include <memory>

#include "graphics/Mesh.hpp"

class MeshFactory
{
public:
    static std::unique_ptr<Mesh> createTriangle();
    static std::unique_ptr<Mesh> createCube();
};