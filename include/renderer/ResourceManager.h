#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <unordered_map>
#include <memory>
#include <string>
#include "Shape.h"
#include "Shader.h"
#include "Texture.h"
#include "linalg/linalg.h"

// ObjectManager that manages shared resources
class ResourceManager {
private:
    std::unordered_map<std::string, std::shared_ptr<Shape>> shapeCache;
    std::unordered_map<std::string, std::shared_ptr<Texture>> textureCache;
    std::unordered_map<std::string, std::shared_ptr<Shader>> shaderCache;

public:
    std::shared_ptr<Shape> getShape(const char* shapeFile, bool loadByIndices = true);

    std::shared_ptr<Texture> getTexture(const char* textureFile);

    std::shared_ptr<Shader> getShader(const char* shaderFile);
};

#endif