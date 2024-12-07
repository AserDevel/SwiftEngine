#ifndef ENTITY_H
#define ENTITY_H

#include "glad/glad.h"
#include "linalg/linalg.h"
#include "Shader.h"
#include "Texture.h"
#include "Shape.h"
#include <memory>

struct InstanceData {
    GLuint instanceID;
    GLuint textureID;
    Mat4x4 matWorld;
};

enum EntityCategory {
    STATIC,
    DYNAMIC
};

// Use this for rendering unique entities like a player, or a tool
class Entity {
public:
    EntityCategory category;  // STATIC or DYNAMIC

    Vec3 position;
    Vec3 direction;
    Vec3 scale;

    bool instanceDataDirty = false;

    std::shared_ptr<Shader> shader;
    std::shared_ptr<Texture> texture;
    std::shared_ptr<Shape> shape;

    std::weak_ptr<Entity> parent;
    std::vector<std::shared_ptr<Entity>> children;

    Entity(std::shared_ptr<Shader> shader, std::shared_ptr<Texture> texture, std::shared_ptr<Shape> shape,
            Vec3 pos = { 0,0,0 }, Vec3 scale = { 1,1,1 }, Vec3 dir = { 0,0,0 }) 
            : shader(shader), texture(texture), shape(shape), position(pos), scale(scale), direction(dir) {}

    InstanceData toInstanceData();

    void updateGPUBuffer();

    void render(Mat4x4 matCamera);

private:
    Mat4x4 getMatWorld();
};

#endif