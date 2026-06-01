#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
    explicit Camera(float aspectRatio);
    ~Camera() = default;

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;

    void setAspectRatio(float aspectRatio);

private:
    glm::vec3 position = { 0.0f, 0.0f, 3.0f };
    glm::vec3 target = { 0.0f, 0.0f, 0.0f };
    glm::vec3 up = { 0.0f, 1.0f, 0.0f };

    float fov = 90.0f;
    float aspectRatio = 1.0f;
    float nearPlane = 0.1f;
    float farPlane = 100.0f;
};