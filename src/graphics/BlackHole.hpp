#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <memory>

class Camera;
class Mesh;
class Shader;


class BlackHole
{
public:
    BlackHole(const glm::vec3 &position, float eventHorizonRadius, float influenceRadius);
    ~BlackHole();

    BlackHole(const BlackHole &) = delete;
    BlackHole &operator=(const BlackHole &) = delete;

    void draw(const Camera &camera, GLuint sceneColorTex, const glm::vec2 &resolution) const;

    void setPosition(const glm::vec3 &newPosition);
    const glm::vec3 &getPosition() const;

private:
    glm::vec3 position;
    float eventHorizonRadius;
    float influenceRadius;

    float distortionStrength = 2.0f;
    float distortionFalloff = 2.0f;
    float maxOffset = 0.35f;

    glm::vec3 ringColor = glm::vec3(1.0f, 0.9f, 0.75f);
    float ringIntensity = 0.6f;

    std::unique_ptr<Mesh> sphere;
    std::unique_ptr<Shader> shader;
};
