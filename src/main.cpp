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

using namespace std;

tuple<unsigned int, unsigned int> generateArrays();
void draw(Shader shader, unsigned int, Texture, Texture);

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "OpenGL SFML");

    window.setActive(true);

    gladLoadGL(reinterpret_cast<GLADloadfunc>(sf::Context::getFunction));
    glEnable(GL_DEPTH_TEST);

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
    shader.setVec3("offset", { 0, 0, 0 });
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

        shader.use();
        shader.setFloat("textureMix", textureMix);

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);

        window.display();
    }

    window.close();

    return 0;
}

void draw(Shader shader, unsigned int vao, Texture texture1, Texture texture2)
{
}
