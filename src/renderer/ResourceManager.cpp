#include "renderer/ResourceManager.h"


std::shared_ptr<Shape> ResourceManager::getShape(const char* shapeFilepath, bool loadByIndices) {
    if (shapeCache.find(shapeFilepath) != shapeCache.end()) {
        return shapeCache[shapeFilepath];
    }
    // Otherwise, load the shape and store it in the cache
    std::shared_ptr<Shape> newShape = std::make_shared<Shape>(shapeFilepath, loadByIndices);
    shapeCache[shapeFilepath] = newShape;
    return newShape;
}

std::shared_ptr<Texture> ResourceManager::getTexture(const char* textureFilepath) {
    if (textureCache.find(textureFilepath) != textureCache.end()) {
        return textureCache[textureFilepath];
    }
    // Otherwise, load the texture and store it in the cache
    std::shared_ptr<Texture> newTexture = std::make_shared<Texture>(textureFilepath);
    textureCache[textureFilepath] = newTexture;
    return newTexture;
}

std::shared_ptr<Shader> ResourceManager::getShader(const char* shaderFilepath) {
    if (shaderCache.find(shaderFilepath) != shaderCache.end()) {
        return shaderCache[shaderFilepath];
    }
    // Otherwise, load the shader program and store it in the cache
    std::shared_ptr<Shader> newShader = std::make_shared<Shader>(shaderFilepath);
    shaderCache[shaderFilepath] = newShader;
    return newShader;
}
