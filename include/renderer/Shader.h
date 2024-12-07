#ifndef SHADER_H
#define SHADER_H

#include "glad/glad.h"
#include "linalg/linalg.h"

struct Shader {
    GLuint programID;

    Shader(const char* shaderFile);

    void use(Mat4x4 matCamera);
};

#endif