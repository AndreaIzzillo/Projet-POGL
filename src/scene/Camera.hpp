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

    void moveForward(float dt);
    void moveBackward(float dt);
    void moveLeft(float dt);
    void moveRight(float dt);
    void moveUp(float dt);
    void moveDown(float dt);
    void rotateLeft(float dt);
    void rotateRight(float dt);
    void rotateUp(float dt);
    void rotateDown(float dt);

private:
    void rotate(float yawDelta, float pitchDelta);

    glm::vec3 position = { 0.0f, 0.0f, 3.0f };
    glm::vec3 front = { 0.0f, 0.0f, -1.0f };
    glm::vec3 worldUp = { 0.0f, 1.0f, 0.0f };
    glm::vec3 up = { 0.0f, 1.0f, 0.0f };
    glm::vec3 right = { 1.0f, 0.0f, 0.0f };

    float movementSpeed = 3.0f;
    float rotationSpeed = 90.0f;

    float fov = 90.0f;
    float aspectRatio = 1.0f;
    float nearPlane = 0.1f;
    float farPlane = 100.0f;
};
