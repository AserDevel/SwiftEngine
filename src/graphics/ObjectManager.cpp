#include "graphics/ObjectManager.h"

std::shared_ptr<ShapeData> ObjectManager::getShape(const char* shapeFilepath) {
    if (shapeCache.find(shapeFilepath) != shapeCache.end()) {
        return shapeCache[shapeFilepath];
    }
    // Otherwise, load the shape and store it in the cache
    std::shared_ptr<ShapeData> newShape = std::make_shared<ShapeData>(shapeFilepath);
    shapeCache[shapeFilepath] = newShape;
    return newShape;
}

std::shared_ptr<Texture> ObjectManager::getTexture(const char* textureFilepath) {
    if (textureCache.find(textureFilepath) != textureCache.end()) {
        return textureCache[textureFilepath];
    }
    // Otherwise, load the texture and store it in the cache
    std::shared_ptr<Texture> newTexture = std::make_shared<Texture>(textureFilepath);
    textureCache[textureFilepath] = newTexture;
    return newTexture;
}

std::shared_ptr<ShaderProgram> ObjectManager::getShaderProgram(const char* shaderFilepath) {
    if (shaderCache.find(shaderFilepath) != shaderCache.end()) {
        return shaderCache[shaderFilepath];
    }
    // Otherwise, load the shader program and store it in the cache
    std::shared_ptr<ShaderProgram> newShader = std::make_shared<ShaderProgram>(shaderFilepath);
    shaderCache[shaderFilepath] = newShader;
    return newShader;
}
