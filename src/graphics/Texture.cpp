#include "graphics/Texture.h"
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>

// Creates a texture from filepath
Texture::Texture(const char* filepath) {
    // Generate textureID
    glGenTextures(1, &textureID);
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
    this->surface = formattedSurface;

    // load texture to GPU so it's ready for rendering.
    loadTextureToGPU();
}

void Texture::loadTextureToGPU() {
    glBindTexture(GL_TEXTURE_2D, textureID);

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
}
    
void TextureArray::loadTexturesToGPU() {
    glBindTexture(GL_TEXTURE_2D_ARRAY, textureArrayID);

    int w = array[0]->surface->w, h = array[0]->surface->h;

    // Allocate storage for the texture array
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, w, h, array.size(), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    // Upload each texture slice into the array
    for (int i = 0; i < array.size(); ++i) {
        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, w, h, 1, GL_RGBA, GL_UNSIGNED_BYTE, array[i]->surface->pixels);
    }

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "Error during texture array upload: " << error << "\n";
    }
}