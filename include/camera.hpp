#pragma once
#include <string>
#include <array>

#include <glad/gl.h>

#include <glm/glm.hpp>
#include <SFML/System/Vector2.hpp>

class Camera
{
public:
    unsigned int ID;

    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;

    glm::vec3 position;
    glm::vec3 direction;

    glm::vec3 up;
    glm::vec3 forward;
    glm::vec3 right;

    float moveSpeed;
    float sensitivity;

    float yaw;
    float pitch;

    Camera();

    void update();

    void rotate(glm::vec2 change);
    void move(glm::vec3 direction);
private:
    glm::vec3 target;
};