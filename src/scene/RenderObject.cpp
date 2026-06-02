#include "scene/RenderObject.hpp"

RenderObject::RenderObject(Mesh *mesh, Shader *shader)
    : mesh(mesh)
    , shader(shader)
{}

void RenderObject::draw(const Camera &camera, float timeSeconds) const
{
    if (!mesh || !shader)
    {
        return;
    }

    shader->use();

    shader->setMat4("uModel", transform.getMatrix());
    shader->setMat4("uView", camera.getViewMatrix());
    shader->setMat4("uProjection", camera.getProjectionMatrix());
    shader->setFloat("uTime", timeSeconds);

    mesh->draw();
}
