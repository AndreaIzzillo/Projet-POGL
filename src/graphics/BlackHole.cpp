#include "graphics/BlackHole.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "graphics/Mesh.hpp"
#include "graphics/MeshFactory.hpp"
#include "graphics/Shader.hpp"
#include "scene/Camera.hpp"

BlackHole::BlackHole(const glm::vec3 &position, float eventHorizonRadius, float influenceRadius)
    : position(position)
    , eventHorizonRadius(eventHorizonRadius)
    , influenceRadius(influenceRadius)
    , sphere(MeshFactory::createSphere(4))
    , shader(std::make_unique<Shader>("shaders/blackhole.vert", "shaders/blackhole.frag"))
{}

BlackHole::~BlackHole() = default;

void BlackHole::setPosition(const glm::vec3 &newPosition)
{
    position = newPosition;
}

const glm::vec3 &BlackHole::getPosition() const
{
    return position;
}

void BlackHole::draw(const Camera &camera, GLuint sceneColorTex, const glm::vec2 &resolution) const
{
    // Cull front faces (draw the inner hemisphere) so the effect keeps rendering when the
    // camera is inside the influence sphere — the OpenGL equivalent of the video's
    // "flip the normals" trick, so you can fly into the black hole.
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
    model = glm::scale(model, glm::vec3(influenceRadius));

    shader->use();
    shader->setMat4("uModel", model);
    shader->setMat4("uView", camera.getViewMatrix());
    shader->setMat4("uProjection", camera.getProjectionMatrix());
    shader->setVec3("uBlackHolePosition", position);
    shader->setFloat("uEventHorizonRadius", eventHorizonRadius);
    shader->setFloat("uInfluenceRadius", influenceRadius);
    shader->setFloat("uDistortionStrength", distortionStrength);
    shader->setFloat("uDistortionFalloff", distortionFalloff);
    shader->setVec3("uGlowColor", glowColor);
    shader->setFloat("uGlowIntensity", glowIntensity);
    shader->setFloat("uGlowFalloff", glowFalloff);

    // The Shader helper has no vec2 / sampler setters, so set these directly.
    const GLint resLoc = glGetUniformLocation(shader->getId(), "uResolution");
    if (resLoc != -1)
        glUniform2f(resLoc, resolution.x, resolution.y);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, sceneColorTex);
    const GLint sceneLoc = glGetUniformLocation(shader->getId(), "uScene");
    if (sceneLoc != -1)
        glUniform1i(sceneLoc, 0);

    sphere->draw();

    // Restore the default render state used by the rest of the scene.
    glCullFace(GL_BACK);
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    glBindTexture(GL_TEXTURE_2D, 0);
}
