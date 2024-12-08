#ifndef SHADER_H
#define SHADER_H

#include "glad/glad.h"
#include "linalg/linalg.h"
#include <unordered_map>
#include <iostream>
#include <string.h>

class Shader {
public:
    GLuint programID;

    Shader(const char* shaderFile);

    ~Shader() {
        if (programID) {
            glUseProgram(0);
            glDeleteProgram(programID);
        } 
    }

    void use();

    void bindMatrix(Mat4x4 matTransform);

    void bindTexture(GLuint textureID);

    void bindTextureArray(GLuint textureArray);

    std::unordered_map<std::string, std::string> loadShadersFromFile(const std::string& filePath);

    GLuint compileShaderProgram(const char* vertexSourceCode, const char* fragmentSourceCode);
};

#endif