#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include "glad/glad.h"

struct ShaderProgram {
    GLuint programID;

    ShaderProgram(const char* shaderFile);

    void use() {
        glUseProgram(programID);
    }
};

#endif