#pragma once
#include <string>
#include <array>

#include <glad/gl.h>

class Texture
{
public:
    unsigned int ID;

    Texture(const char* fragmentPath);

    void use(GLenum index);
};