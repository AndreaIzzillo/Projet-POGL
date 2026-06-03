#include "scene/RenderObject.hpp"

RenderObject::RenderObject(Mesh *mesh, Shader *shader, bool isTransparent, bool reverseCullFace)
    : mesh(mesh)
    , shader(shader)
    , isTransparent(isTransparent)
    , reverseCullFace(reverseCullFace)
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
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthMask(GL_FALSE);
    }

    if (reverseCullFace)
    {
        glCullFace(GL_FRONT);
    }

    shader->use();

    shader->setMat4("uModel", transform.getMatrix());
    shader->setMat4("uView", camera.getViewMatrix());
    shader->setMat4("uProjection", camera.getProjectionMatrix());
    shader->setVec3("uCameraPosition", camera.getViewMatrix()[3]);
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
}
