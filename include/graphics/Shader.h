#ifndef SHADER_H
#define SHADER_H

#include "glad/glad.h"
#include "linalg/linalg.h"
#include <unordered_map>
#include <iostream>
#include <string.h>
#include <vector>

struct LightSource;

class Shader {
    GLuint programID;

    std::unordered_map<std::string, std::string> loadShadersFromFile(const std::string& filePath);

    GLuint compileShaderProgram(const char* vertexSourceCode, const char* fragmentSourceCode);

public:
    Shader(const char* shaderFile);

    ~Shader() {
        if (programID) {
            glUseProgram(0);
            glDeleteProgram(programID);
            programID = 0;
        } 
    }

    void use();

    // Uniform binding functions
    void bindMatrix(Mat4x4 matrix, const char* name);

    void bindTexture(GLuint textureID);

    void bindTextureArray(GLuint textureArrayID);

    void bindVector(Vec3 vector, const char* name);

    void bindFloat(float f, const char* name);

    void bindLights(std::vector<LightSource> lights);
};

#endif