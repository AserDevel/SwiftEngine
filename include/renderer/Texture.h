#ifndef TEXTURE_H
#define TEXTURE_H

#include "glad/glad.h"
#include <vector>
#include <memory>
#include <SDL2/SDL.h>

class Texture {
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

    void loadToGPU();
};

class TextureArray {
public:   
    std::vector<std::shared_ptr<Texture>> array;
    GLuint textureArrayID;

    TextureArray() {}

    ~TextureArray() {
        if (textureArrayID) {
            glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
            glDeleteTextures(1, &textureArrayID);
        }        
    }

    void addTexture(std::shared_ptr<Texture> texture);

    void loadToGPU();
};


#endif