#pragma once

#include "graphics/Mesh.hpp"
#include "graphics/Shader.hpp"
#include "scene/Camera.hpp"
#include "scene/Transform.hpp"

class RenderObject
{
public:
    RenderObject(Mesh *mesh, Shader *shader, bool isTransparent = false,
                 bool reverseCullFace = false);

    void draw(const Camera &camera, float timeSeconds) const;

    Transform transform;

private:
    Mesh *mesh = nullptr;
    Shader *shader = nullptr;
    bool isTransparent = false;
    bool reverseCullFace = false;
};
