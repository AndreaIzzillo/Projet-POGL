#include "scene/RenderObject.hpp"

glm::vec3 RenderObject::sunPosition = glm::vec3(0.0f);

void RenderObject::setSunPosition(const glm::vec3 &position)
{
    sunPosition = position;
}

RenderObject::RenderObject(Mesh *mesh, Shader *shader, bool isTransparent, bool reverseCullFace,
                           bool disableCulling)
    : mesh(mesh)
    , shader(shader)
    , isTransparent(isTransparent)
    , reverseCullFace(reverseCullFace)
    , disableCulling(disableCulling)
{}

void RenderObject::draw(const Camera &camera, float timeSeconds) const
{
    if (!mesh || !shader)
    {
        return;
    }

    if (isTransparent)
    {
        glEnable(GL_BLEND);
        glDepthMask(GL_FALSE);
    }

    if (reverseCullFace)
    {
        glCullFace(GL_FRONT);
    }

    if (disableCulling)
    {
        glDisable(GL_CULL_FACE);
    }

    shader->use();

    shader->setMat4("uModel", transform.getMatrix());
    shader->setMat4("uView", camera.getViewMatrix());
    shader->setMat4("uProjection", camera.getProjectionMatrix());
    shader->setVec3("uCameraPosition", camera.getViewMatrix()[3]);
    shader->setVec3("uSunPosition", sunPosition);
    shader->setFloat("uTime", timeSeconds);

    mesh->draw();

    if (isTransparent)
    {
        glDisable(GL_BLEND);
        glDepthMask(GL_TRUE);
    }
    if (reverseCullFace)
    {
        glCullFace(GL_BACK);
    }

    if (disableCulling)
    {
        glEnable(GL_CULL_FACE);
    }
}
