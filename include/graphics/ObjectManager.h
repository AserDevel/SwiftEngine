#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include <unordered_map>
#include <memory>
#include <string>
#include "ShapeData.h"
#include "Object.h"
#include "linalg/linalg.h"

// ObjectManager that manages shared resources
class ObjectManager {
private:
    std::unordered_map<std::string, std::shared_ptr<ShapeData>> shapeCache;
    std::unordered_map<std::string, std::shared_ptr<Texture>> textureCache;
    std::unordered_map<std::string, std::shared_ptr<ShaderProgram>> shaderCache;

public:
    std::shared_ptr<ShapeData> getShape(const char* shapeFile);

    std::shared_ptr<Texture> getTexture(const char* textureFile);

    std::shared_ptr<ShaderProgram> getShaderProgram(const char* shaderFile);
};

#endif