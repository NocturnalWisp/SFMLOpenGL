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

    sf::Clock clock;
    sf::Clock deltaClock;

    gladLoadGL(reinterpret_cast<GLADloadfunc>(sf::Context::getFunction));

    glEnable(GL_DEPTH_TEST);

    Camera cam = Camera();

    Shader cubeShader = Shader("resources/triangle_vertex.vs", "resources/triangle_fragment.fs");
    Shader lampShader = Shader("resources/light_vertex.vs", "resources/light_fragment.fs");

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int lightvao;
    glGenVertexArrays(1, &lightvao);
    glBindVertexArray(lightvao);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    Texture texture = Texture("resources/download.jfif");
    Texture texture2 = Texture("resources/face.png");

    cubeShader.use();
    cubeShader.setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
    cubeShader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

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
                    isRunning = false;
                }
            }
            else if (event.type == sf::Event::MouseMoved)
            {
                sf::Vector2f mouseOffset = lastMousePos - (sf::Vector2f)sf::Mouse::getPosition();
                sf::Mouse::setPosition((sf::Vector2i)window.getSize());
                cam.rotate(glm::vec2(mouseOffset.x, mouseOffset.y));

                lastMousePos = (sf::Vector2f)sf::Mouse::getPosition();
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

        glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

        glClearColor(0.1, 0.1, 0.1, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        texture.use(GL_TEXTURE0);
        texture2.use(GL_TEXTURE1);

        cubeShader.use();
        cubeShader.setMat4("view", glm::value_ptr(cam.viewMatrix));
        cubeShader.setMat4("projection", glm::value_ptr(cam.projectionMatrix));
        cubeShader.setVec3("lightPos", lightPos);
        cubeShader.setVec3("viewPos", cam.position);

        glBindVertexArray(vao);

        auto modelMatrix = glm::mat4(1.0f);
        cubeShader.setMat4("model", glm::value_ptr(modelMatrix));

        glDrawArrays(GL_TRIANGLES, 0, 36);

        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, lightPos);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));

        lampShader.use();
        lampShader.setMat4("view", glm::value_ptr(cam.viewMatrix));
        lampShader.setMat4("projection", glm::value_ptr(cam.projectionMatrix));
        lampShader.setMat4("model", glm::value_ptr(modelMatrix));

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
