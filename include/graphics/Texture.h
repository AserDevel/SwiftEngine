#ifndef TEXTURE_H
#define TEXTURE_H

#include "glad/glad.h"
#include <vector>
#include <memory>
#include <SDL2/SDL.h>

class Texture {
    void loadTextureToGPU();

public:
    GLuint textureID;

    SDL_Surface* surface;
   
    Texture(const char* textureFile);

    ~Texture() {
        if (textureID) {
            glBindTexture(GL_TEXTURE_2D, 0);
            glDeleteTextures(1, &textureID);
        }
        SDL_FreeSurface(surface);
    }    
};

class TextureArray {
public:   
    GLuint textureArrayID;

    std::vector<std::shared_ptr<Texture>> array;

    TextureArray() {
        // Generate ID
        glGenTextures(1, &textureArrayID);
    }

    ~TextureArray() {
        // Cleanup
        if (textureArrayID) {
            glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
            glDeleteTextures(1, &textureArrayID);
        }        
    }

    // Loads the current array into GPU overwriting the last one
    void loadTexturesToGPU();
};


#endif