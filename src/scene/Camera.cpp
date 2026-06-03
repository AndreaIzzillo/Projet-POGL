#include "scene/Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(float aspectRatio)
    : aspectRatio(aspectRatio)
{}

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::getProjectionMatrix() const
{
    return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

void Camera::setAspectRatio(float newAspectRatio)
{
    aspectRatio = newAspectRatio;
}

void Camera::moveForward(float dt)
{
    position += front * movementSpeed * dt;
}

void Camera::moveBackward(float dt)
{
    position -= front * movementSpeed * dt;
}

void Camera::moveLeft(float dt)
{
    position -= right * movementSpeed * dt;
}

void Camera::moveRight(float dt)
{
    position += right * movementSpeed * dt;
}

void Camera::moveUp(float dt)
{
    position += worldUp * movementSpeed * dt;
}

void Camera::moveDown(float dt)
{
    position -= worldUp * movementSpeed * dt;
}

void Camera::rotateLeft(float dt)
{
    rotate(rotationSpeed * dt, 0.0f);
}

void Camera::rotateRight(float dt)
{
    rotate(-rotationSpeed * dt, 0.0f);
}

void Camera::rotateUp(float dt)
{
    rotate(0.0f, rotationSpeed * dt);
}

void Camera::rotateDown(float dt)
{
    rotate(0.0f, -rotationSpeed * dt);
}

void Camera::rotateByMouse(float dx, float dy)
{
    rotate(-dx * mouseSensitivity, -dy * mouseSensitivity);
}

void Camera::rotate(float yawDelta, float pitchDelta)
{
    if (yawDelta != 0.0f)
    {
        const float yawRadians = glm::radians(yawDelta);
        const glm::mat3 rotation = glm::mat3(glm::rotate(glm::mat4(1.0f), yawRadians, worldUp));
        front = glm::normalize(rotation * front);
        right = glm::normalize(rotation * right);
    }

    if (pitchDelta != 0.0f)
    {
        const float pitchRadians = glm::radians(pitchDelta);
        const glm::mat3 rotation = glm::mat3(glm::rotate(glm::mat4(1.0f), pitchRadians, right));
        const glm::vec3 candidateFront = glm::normalize(rotation * front);

        if (glm::abs(glm::dot(candidateFront, worldUp)) < 0.99f)
        {
            front = candidateFront;
        }
    }

    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}

void Camera::increaseSpeed(float dt)
{
    movementSpeed += 500.0f * dt;
}

void Camera::decreaseSpeed(float dt)
{
    movementSpeed = std::max(0.0f, movementSpeed - 500.0f * dt);
}