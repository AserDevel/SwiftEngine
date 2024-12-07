#ifndef TEXTURE_H
#define TEXTURE_H

#include "glad/glad.h"

struct Texture {
    GLuint textureID;

    Texture(const char* textureFile);

    void bind(GLuint programID);
};

#endif