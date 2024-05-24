#include "camera.hpp"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera()
{
    projectionMatrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    // projectionMatrix = glm::ortho(-1.3333f, 1.3333f, -1.0f, 1.0f, 0.1f, 100.0f);

    position = glm::vec3(1.0f, 2.0f, 5.0f);
    target = glm::vec3(0.0f, 0.0f, 0.0f);
    direction = glm::normalize(position - target);
    moveSpeed = 0.005f;
    sensitivity = 0.1f;

    up = glm::vec3(0.0f, 1.0f, 0.0f);
    right = glm::normalize(glm::cross(up, direction));

    forward = glm::vec3(0.0f, 0.0f, -1.0f);

    yaw = -90.0f;
    pitch = 0.0f;

    viewMatrix = glm::mat4(1.0f);
}

void Camera::update()
{
    forward = glm::normalize(glm::vec3(
        cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
        sin(glm::radians(pitch)),
        sin(glm::radians(yaw)) * cos(glm::radians(pitch))
    ));

    viewMatrix = glm::lookAt(position, position + forward, up);
}

void Camera::rotate(glm::vec2 change)
{
    change *= sensitivity;

    yaw -= change.x;
    pitch += change.y;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;
}

void Camera::move(glm::vec3 direction)
{
    position += glm::normalize(direction) * moveSpeed;
}