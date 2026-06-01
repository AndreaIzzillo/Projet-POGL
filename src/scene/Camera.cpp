#include "scene/Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(float aspectRatio)
    : aspectRatio(aspectRatio)
{}

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(position, target, up);
}

glm::mat4 Camera::getProjectionMatrix() const
{
    return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

void Camera::setAspectRatio(float newAspectRatio)
{
    aspectRatio = newAspectRatio;
}
