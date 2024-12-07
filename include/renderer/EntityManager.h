#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "EntityBatch.h"

// Manages all entites, sorts batches by shader -> shape -> texture for effecient GPU rendering
class EntityManager {
    static GLuint globalInstanceCounter;

    std::vector<std::unique_ptr<Entity>> entities;
    std::vector<std::unique_ptr<EntityBatch>> batches;

public:
    std::unique_ptr<Entity> createEntity(std::shared_ptr<Shader> shader, std::shared_ptr<Texture> texture,
                         std::shared_ptr<Shape> shape, Vec3 pos, Vec3 dir, Vec3 scale, bool dynamic);

    std::unique_ptr<EntityBatch> createEntityBatch(std::shared_ptr<Shader> shader, std::shared_ptr<Shape> shape);
    
    void renderAllEntities(Mat4x4 matViewProj);
};


#endif