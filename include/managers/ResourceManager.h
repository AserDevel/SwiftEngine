#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <unordered_map>
#include <memory>
#include <string>
#include "graphics/Shape.h"
#include "graphics/Shader.h"
#include "graphics/Texture.h"
#include "linalg/linalg.h"

// ObjectManager that manages shared resources
class ResourceManager {
private:
    ResourceManager() {}

    std::unordered_map<std::string, std::weak_ptr<Shape>> shapeCache;
    std::unordered_map<std::string, std::weak_ptr<Texture>> textureCache;
    std::unordered_map<std::string, std::weak_ptr<Shader>> shaderCache;

public:
    static ResourceManager& getInstance() {
        static ResourceManager instance;
        return instance;
    }

    // Delete copy constructor and assignment operator
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    std::shared_ptr<Shape> getShape(const char* shapeFile, bool loadByIndices = true);

    std::shared_ptr<Texture> getTexture(const char* textureFile);

    std::shared_ptr<Shader> getShader(const char* shaderFile);
};

#endif