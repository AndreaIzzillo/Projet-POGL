#include "scene/RenderObject.hpp"

glm::vec3 RenderObject::sunPosition = glm::vec3(0.0f);

void RenderObject::setSunPosition(const glm::vec3 &position)
{
    sunPosition = position;
}

RenderObject::RenderObject(Mesh *mesh, Shader *shader, bool isTransparent)
    : mesh(mesh)
    , shader(shader)
    , isTransparent(isTransparent)
{}

bool RenderObject::isTransparentObject() const
{
    return isTransparent;
}

float RenderObject::getDistanceToCamera(const Camera &camera) const
{
    return glm::length(transform.position - camera.getPosition());
}

void RenderObject::setBeforeDraw(StateFunc beforeDrawFunc)
{
    beforeDraw = beforeDrawFunc;
}

void RenderObject::setAfterDraw(StateFunc afterDrawFunc)
{
    afterDraw = afterDrawFunc;
}

void RenderObject::draw(const Camera &camera, float timeSeconds, float supernovaTime) const
{
    if (!mesh || !shader)
        return;

    shader->use();

    shader->setMat4("uModel", transform.getMatrix());
    shader->setMat4("uView", camera.getViewMatrix());
    shader->setMat4("uProjection", camera.getProjectionMatrix());
    shader->setVec3("uCameraPosition", camera.getPosition());
    shader->setVec3("uSunPosition", sunPosition);
    shader->setFloat("uTime", timeSeconds);
    shader->setFloat("uSupernovaTime", supernovaTime);

    if (beforeDraw)
        beforeDraw();

    mesh->draw();

    if (afterDraw)
        afterDraw();
}
