#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "EntityBatch.h"

// Manages all entites, sorts batches by shader -> shape -> texture for effecient GPU rendering
class EntityManager {
    std::vector<std::unique_ptr<Entity>> entities;
    std::vector<std::unique_ptr<EntityBatch>> batches;

public:
    Entity* createEntity(
        std::shared_ptr<Shader> shader, 
        std::shared_ptr<Shape> shape, 
        std::shared_ptr<Texture> texture, 
        Vec3 pos = { 0,0,0 }, 
        Vec3 scale = { 1,1,1 }, 
        Vec3 dir = { 0,0,0 });

    EntityBatch* createEntityBatch(std::shared_ptr<Shader> shader, std::shared_ptr<Shape> shape);
    
    void renderAllEntities(Mat4x4 matCamera);
};


#endif