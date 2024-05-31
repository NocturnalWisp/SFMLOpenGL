#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iostream>

#include <glad/gl.h>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>

#include "shader.hpp"
#include "texture.hpp"
#include "camera.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

using namespace std;

int main()
{
    sf::ContextSettings settings = sf::ContextSettings(24);
    sf::RenderWindow window(sf::VideoMode(800, 600), "OpenGL SFML", sf::Style::Default, settings);

    window.setActive(true);

    window.setMouseCursorVisible(false);
    window.setMouseCursorGrabbed(true);
    bool mouseDisabled = true;

    sf::Clock clock;
    sf::Clock deltaClock;

    gladLoadGL(reinterpret_cast<GLADloadfunc>(sf::Context::getFunction));

    glEnable(GL_DEPTH_TEST);

    Camera cam = Camera();

    Shader cubeShader = Shader("resources/triangle_vertex.vs", "resources/triangle_fragment.fs");
    Shader lampShader = Shader("resources/light_vertex.vs", "resources/light_fragment.fs");

    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    unsigned int vbo, vao, ebo;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    // glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int lightvao;
    glGenVertexArrays(1, &lightvao);
    glBindVertexArray(lightvao);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    Texture diffuseMap = Texture("resources/diffuse_map.png");
    Texture specularMap = Texture("resources/specular_map.png");

    cubeShader.use();
    cubeShader.setInt("material.diffuse", 0);
    cubeShader.setInt("material.specular", 1);
    cubeShader.setFloat("material.shininess", 64.0f);

    // cubeShader.setVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
    // cubeShader.setVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
    cubeShader.setVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
    cubeShader.setVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
    cubeShader.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

    glBindVertexArray(0);

    bool isRunning = true;
    float deltaTime = 0;

    sf::Vector2f lastMousePos = (sf::Vector2f)sf::Mouse::getPosition();

    while (isRunning)
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                isRunning = false;
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                {
                    if (mouseDisabled)
                    {
                        window.setMouseCursorGrabbed(false);
                        window.setMouseCursorVisible(true);
                        mouseDisabled = false;
                    }
                    else
                    {
                        isRunning = false;
                    }
                }
            }
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                if (!mouseDisabled)
                {
                    window.setMouseCursorGrabbed(true);
                    window.setMouseCursorVisible(false);
                    mouseDisabled = true;

                    lastMousePos = (sf::Vector2f)sf::Mouse::getPosition();
                }
            }
            else if (event.type == sf::Event::MouseMoved)
            {
                if (mouseDisabled)
                {
                    sf::Vector2f mouseOffset = lastMousePos - (sf::Vector2f)sf::Mouse::getPosition();
                    sf::Mouse::setPosition((sf::Vector2i)window.getSize());
                    cam.rotate(glm::vec2(mouseOffset.x, mouseOffset.y));

                    lastMousePos = (sf::Vector2f)sf::Mouse::getPosition();
                }
            }
        }

        cam.moveSpeed = 2.5f * deltaTime;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            cam.move(cam.forward);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            cam.move(-cam.forward);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            cam.move(glm::cross(cam.forward, cam.up));
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            cam.move(-glm::cross(cam.forward, cam.up));

        cam.update();

        auto cubeMatrix = glm::mat4(1.0f);

        glm::mat4 lampMatrix = glm::mat4(1.0f);
        lampMatrix = glm::translate(cubeMatrix, glm::vec3(sin(clock.getElapsedTime().asSeconds() * 0.1f), 0.0, cos(clock.getElapsedTime().asSeconds() * 0.1f)));
        lampMatrix = glm::scale(lampMatrix, glm::vec3(0.2f));
        glm::vec3 lightPos = glm::vec3(lampMatrix[3]);

        glClearColor(0.1, 0.1, 0.1, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        diffuseMap.use(GL_TEXTURE0);
        specularMap.use(GL_TEXTURE1);

        cubeShader.use();
        cubeShader.setMat4("view", glm::value_ptr(cam.viewMatrix));
        cubeShader.setMat4("projection", glm::value_ptr(cam.projectionMatrix));
        cubeShader.setVec3("light.position", lightPos);
        cubeShader.setVec3("viewPos", cam.position);

        glBindVertexArray(vao);

        cubeShader.setMat4("model", glm::value_ptr(cubeMatrix));

        glDrawArrays(GL_TRIANGLES, 0, 36);

        lampShader.use();
        lampShader.setMat4("view", glm::value_ptr(cam.viewMatrix));
        lampShader.setMat4("projection", glm::value_ptr(cam.projectionMatrix));
        lampShader.setMat4("model", glm::value_ptr(lampMatrix));

        glBindVertexArray(lightvao);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(0);

        window.display();

        deltaTime = deltaClock.restart().asSeconds();
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    // glDeleteBuffers(1, &ebo);

    window.close();

    return 0;
}
