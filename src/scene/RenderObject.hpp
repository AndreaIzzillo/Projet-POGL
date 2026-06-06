#pragma once

#include <glm/glm.hpp>

#include "graphics/Mesh.hpp"
#include "graphics/Shader.hpp"
#include "scene/Camera.hpp"
#include "scene/Transform.hpp"

class RenderObject
{
public:
    RenderObject(Mesh *mesh, Shader *shader, bool isTransparent = false);

    bool isTransparentObject() const;
    float getDistanceToCamera(const Camera &camera) const;

    typedef void (*StateFunc)();

    void setBeforeDraw(StateFunc beforeDrawFunc);
    void setAfterDraw(StateFunc afterDrawFunc);

    void draw(const Camera &camera, float timeSeconds, float supernovaTime) const;

    static void setSunPosition(const glm::vec3 &sunPosition);

    Transform transform;

private:
    Mesh *mesh = nullptr;
    Shader *shader = nullptr;

    StateFunc beforeDraw = nullptr;
    StateFunc afterDraw = nullptr;

    bool isTransparent = false;

    static glm::vec3 sunPosition;
};
