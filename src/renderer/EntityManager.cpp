#include "renderer/EntityManager.h"

std::unique_ptr<Entity> EntityManager::createEntity(std::shared_ptr<Shader> shader, std::shared_ptr<Texture> texture,
                                std::shared_ptr<Shape> shape, Vec3 pos, Vec3 dir, Vec3 scale, bool dynamic = true) {
    
}

std::unique_ptr<EntityBatch> EntityManager::createEntityBatch(std::shared_ptr<Shader> shader, std::shared_ptr<Shape> shape) {

}

void EntityManager::renderAllEntities(Mat4x4 matViewProj) {

}