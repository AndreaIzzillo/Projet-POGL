#pragma once

#include "graphics/Mesh.hpp"
#include "graphics/Shader.hpp"
#include "scene/Camera.hpp"
#include "scene/Transform.hpp"

class RenderObject
{
public:
    RenderObject(Mesh *mesh, Shader *shader);

    void draw(const Camera &camera) const;

    Transform transform;

private:
    Mesh *mesh = nullptr;
    Shader *shader = nullptr;
};