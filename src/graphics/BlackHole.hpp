#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <memory>

class Camera;
class Mesh;
class Shader;

// A screen-space gravitational-lensing black hole (Outer Wilds style).
// It is drawn last, after the scene has been captured into a texture: the shader samples
// that texture with UVs pushed toward the black hole center to fake the lensing, draws a
// black event-horizon disk, and adds a faint fresnel rim glow.
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

    // Look / lensing parameters (tweak to taste).
    float distortionStrength = 2.0f;
    float distortionFalloff = 2.0f;
    glm::vec3 glowColor = glm::vec3(0.4f, 0.6f, 1.0f);
    float glowIntensity = 0.15f;
    float glowFalloff = 3.0f;

    std::unique_ptr<Mesh> sphere;
    std::unique_ptr<Shader> shader;
};
