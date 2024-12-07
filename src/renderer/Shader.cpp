#include "renderer/Shader.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>

void Shader::use(Mat4x4 matCamera) {
    glUseProgram(programID);

    GLuint uniformLocation = glGetUniformLocation(programID, "matCamera");

	if (uniformLocation == -1) {
		std::cerr << "Uniform 'matCamera' not found in shader program\n";
	} else {
		glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, &matCamera[0][0]);
		GLenum error = glGetError();
		if (error != GL_NO_ERROR) {
			std::cerr << "Error while setting uniform 'matCamera': " << error << "\n";
		}
	}
}

// Function to load and split shaders
std::unordered_map<std::string, std::string> loadShaderFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open shader file: " + filePath);
    }

    enum ShaderType { NONE = -1, VERTEX, FRAGMENT };
    ShaderType currentType = NONE;

    std::unordered_map<std::string, std::string> shaderSources;
    shaderSources["vertex"] = "";
    shaderSources["fragment"] = "";

    std::string line;
    std::stringstream ss[2]; // Two streams for vertex and fragment shaders

    while (std::getline(file, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                currentType = VERTEX;
            } else if (line.find("fragment") != std::string::npos) {
                currentType = FRAGMENT;
            }
        } else {
            if (currentType != NONE) {
                ss[currentType] << line << '\n';
            }
        }
    }

    shaderSources["vertex"] = ss[VERTEX].str();
    shaderSources["fragment"] = ss[FRAGMENT].str();
    return shaderSources;
}

// compile shader program from source code
GLuint compileShaderProgram(std::string sVertexSourceCode, std::string sFragmentSourceCode) {
	GLuint vertexShader;
    GLuint fragmentShader;
    int success;
    char infoLog[512];
    const char* vertexSourceCode = sVertexSourceCode.c_str();
    if (vertexSourceCode != NULL) {
        // Compile Vertex Shader
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexSourceCode, NULL);
        glCompileShader(vertexShader);

        // Check for compilation errors
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cerr << "Error compiling vertex shader: " << infoLog << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    const char* fragmentSourceCode = sFragmentSourceCode.c_str();
    if (fragmentSourceCode != NULL) {
        // Compile Fragment Shader
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentSourceCode, NULL);
        glCompileShader(fragmentShader);

        // Check for compilation errors
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cerr << "Error compiling fragment shader: " << infoLog << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    
    // Link shaders into a shader program
    GLuint shaderProgram = glCreateProgram();
    if (vertexShader) glAttachShader(shaderProgram, vertexShader);
    if (fragmentShader) glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Cleanup shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "Error linking shader program: " << infoLog << std::endl;
        exit(EXIT_FAILURE);
    }

    return shaderProgram;
}

// load and compile shader program from file
Shader::Shader(const char* shaderFile) {  
    std::unordered_map<std::string, std::string> shaders = loadShaderFromFile(shaderFile);
    this->programID = compileShaderProgram(shaders["vertex"], shaders["fragment"]);
}