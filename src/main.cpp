#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iostream>

#include <glad/gl.h>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>

#include <shader.hpp>
#include <texture.hpp>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

using namespace std;

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "OpenGL SFML");

    window.setActive(true);

    sf::Clock clock;

    gladLoadGL(reinterpret_cast<GLADloadfunc>(sf::Context::getFunction));
    glEnable(GL_DEPTH_TEST);

    glm::mat4 trans = glm::mat4(1.0);
    trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
    trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
    trans = glm::translate(trans, glm::vec3(0.5, -0.5, 0.5));

    Shader shader = Shader("resources/triangle_vertex.glsl", "resources/triangle_fragment.glsl");

    float vertices[] = {
        // Verts,         Colors,         Tex
        0.5,  0.5, 0.0,   1.0, 0.0, 0.0,  1.0, 2.0,
        0.5, -0.5, 0.0,   0.0, 1.0, 0.0,  1.0, 0.0,
       -0.5, -0.5, 0.0,   0.0, 0.0, 1.0,  0.0, 0.0,
       -0.5,  0.5, 0.0,   1.0, 1.0, 0.0,  0.0, 2.0
    };

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    unsigned int vbo, vao, ebo;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    Texture texture = Texture("resources/download.jfif");
    Texture texture2 = Texture("resources/face.png");

    shader.use();
    shader.setInt("texture1", 0);
    shader.setInt("texture2", 1);

    glBindVertexArray(0);

    float textureMix = 0.3;

    bool isRunning = true;
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
                if (event.key.code == sf::Keyboard::Up)
                {
                    textureMix += 0.1;
                }
                if (event.key.code == sf::Keyboard::Down)
                {
                    textureMix -= 0.1;
                }
            }
        }

        glClearColor(0.2, 0.3, 0.3, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        texture.use(GL_TEXTURE0);
        texture2.use(GL_TEXTURE1);

        trans = glm::rotate(trans, glm::radians(0.03f), glm::vec3(0.0f, 0.0f, 1.0f));

        shader.use();
        shader.setFloat("textureMix", textureMix);
        shader.setMat4("transform", glm::value_ptr(trans));

        glBindVertexArray(vao);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glm::mat4 newTrans = glm::mat4(1.0);
        newTrans = glm::translate(newTrans, glm::vec3(-0.5, 0.5, 0));

        shader.setFloat("textureMix", 0.4);
        shader.setMat4("transform", glm::value_ptr(newTrans));

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);

        window.display();
    }

    window.close();

    return 0;
}
