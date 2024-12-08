#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>
#include "renderer/Shader.h"

void Shader::use() {
    glUseProgram(programID);
}

void Shader::bindMatrix(Mat4x4 matTransform) {
    GLuint uniformLocation = glGetUniformLocation(programID, "matTransform");
	if (uniformLocation == -1) {
		std::cerr << "Uniform 'matTransform' not found in shader program\n";
	} else {
		glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, &matTransform[0][0]);
		GLenum error = glGetError();
		if (error != GL_NO_ERROR) {
			std::cerr << "Error while setting uniform 'matTransform': " << error << "\n";
		}
	}
}

void Shader::bindTexture(GLuint textureID) {
    // Bind the texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Get the uniform location
    GLuint uniformLocation = glGetUniformLocation(programID, "textureData");
    glUniform1i(uniformLocation, 0);  // Set texture unit 0 (first texture)
	if (uniformLocation == -1) {
		std::cerr << "Uniform 'textureData' not found in shader program\n";
	} else {
		glUniform1i(uniformLocation, 0); // Corresponds to GL_TEXTURE0
		GLenum error = glGetError();
		if (error != GL_NO_ERROR) {
			std::cerr << "Error while setting uniform 'textureData': " << error << "\n";
		}
	}
}

void Shader::bindTextureArray(GLuint textureArrayID) {
    glActiveTexture(GL_TEXTURE0); // Use texture unit 0 (first texture)
    glBindTexture(GL_TEXTURE_2D_ARRAY, textureArrayID);

    GLuint uniformLocation = glGetUniformLocation(programID, "textureArray");
	if (uniformLocation == -1) {
		std::cerr << "Uniform 'textureArray' not found in shader program\n";
	} else {
		glUniform1i(uniformLocation, 0); // Corresponds to GL_TEXTURE0
		GLenum error = glGetError();
		if (error != GL_NO_ERROR) {
			std::cerr << "Error while setting uniform 'textureArray': " << error << "\n";
		}
	}
}

// Function to load and split shaders
std::unordered_map<std::string, std::string> Shader::loadShadersFromFile(const std::string& filePath) {
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
GLuint Shader::compileShaderProgram(const char* vertexSourceCode, const char* fragmentSourceCode) {
    GLuint vertexShader = 0;
    GLuint fragmentShader = 0;
    int success;
    char infoLog[512];

    // Compile Vertex Shader if provided
    if (vertexSourceCode != nullptr) {
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexSourceCode, NULL);
        glCompileShader(vertexShader);
        
        // Check for compilation errors
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cerr << "Error compiling vertex shader: " << infoLog << std::endl;
            glDeleteShader(vertexShader);  // Clean up the shader
            return 0;  // Return 0 to indicate error
        }
    } else {
        std::cout << "vertexSourceCode was null" << std::endl;
    }

    // Compile Fragment Shader if provided
    if (fragmentSourceCode != nullptr) {
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentSourceCode, NULL);
        glCompileShader(fragmentShader);
        
        // Check for compilation errors
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cerr << "Error compiling fragment shader: " << infoLog << std::endl;
            glDeleteShader(fragmentShader);  // Clean up the shader
            if (vertexShader) glDeleteShader(vertexShader); // Cleanup vertex shader if it was compiled
            return 0;  // Return 0 to indicate error
        }
    } else {
        std::cout << "fragmentSourceCode was null" << std::endl;
    }

    // Link shaders into a shader program
    GLuint shaderProgram = glCreateProgram();
    if (vertexShader) glAttachShader(shaderProgram, vertexShader);
    if (fragmentShader) glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Cleanup shaders after linking
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "Error linking shader program: " << infoLog << std::endl;
        glDeleteProgram(shaderProgram); // Cleanup the program on error
        return 0;  // Return 0 to indicate error
    }

    return shaderProgram;
}

// load and compile shader program from file
Shader::Shader(const char* shaderFile) {
    std::unordered_map<std::string, std::string> shaders = loadShadersFromFile(shaderFile);
    
    const char* vertexShaderSource = shaders["vertex"].c_str();
    const char* fragmentShaderSource = shaders["fragment"].c_str();
    
    GLuint compiledProgram = compileShaderProgram(vertexShaderSource, fragmentShaderSource);
    if (compiledProgram == 0) {
        std::cerr << "Shader compilation or linking failed!" << std::endl;
    }

    this->programID = compiledProgram;
}
