#include "renderer/Texture.h"
#include <SDL2/SDL_image.h>
#include <iostream>

// Creates a texture from filepath
Texture::Texture(const char* filepath) {
	// Load image using SDL_image
    SDL_Surface* surface = IMG_Load(filepath);
    if (!surface) {
        std::cerr << "Error loading image: " << filepath << " - " << IMG_GetError() << std::endl;
        return;
    }

    // Convert the surface to OpenGL texture format (RGBA)
    SDL_Surface* formattedSurface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
    if (formattedSurface->format->format != SDL_PIXELFORMAT_RGBA32) {
        std::cerr << "Error converting surface to RGBA32: " << filepath << std::endl;
    }
    SDL_FreeSurface(surface);
    surface = formattedSurface;

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Set texture parameters (filtering, wrapping)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Upload the texture to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
    glGenerateMipmap(GL_TEXTURE_2D);

    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "Error during texture upload: " << error << "\n";
    }

    // Free SDL surface (it is no longer needed after the texture is uploaded)
    SDL_FreeSurface(surface);

    textureID = texture;
}

// Binds uniform texuture to programID
void Texture::bind(GLuint programID) {
    if (textureID) {
		// Get the location of the texture uniform in the fragment shader
    	GLuint textureLocation = glGetUniformLocation(programID, "textureData");
    	glUniform1i(textureLocation, 0);  // Set texture unit 0 (first texture)

    	// Bind the texture (bind the texture before drawing)
    	glActiveTexture(GL_TEXTURE0);
    	glBindTexture(GL_TEXTURE_2D, textureID);

        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            std::cerr << "Error during texture binding: " << error << "\n";
        }
    }
}