#include "renderer/EntityManager.h"

// Creates a new entity and adds it to the list of entities
Entity* EntityManager::createEntity(
    std::shared_ptr<Shader> shader,
    std::shared_ptr<Shape> shape,
    std::shared_ptr<Texture> texture, 
    Vec3 pos, Vec3 scale, Vec3 dir) {
    // Create a new entity and set its position, scale, and direction
    auto entity = std::make_unique<Entity>(shader, shape, texture, pos, scale, dir);

    // Add the entity to the list of entities
    entities.push_back(std::move(entity));
    
    // Return the new entity
    return entities.back().get();
}

// Creates an EntityBatch with a given shader and shape (batches entities with the same shader and shape)
EntityBatch* EntityManager::createEntityBatch(std::shared_ptr<Shader> shader, std::shared_ptr<Shape> shape) {
    // Create a new EntityBatch for the given shader and shape
    auto batch = std::make_unique<EntityBatch>(shader, shape);

    // Add it to the list of batches
    batches.push_back(std::move(batch));

    // Return the new batch
    return batches.back().get();
}

// Render all entities by iterating over the batches and rendering each
void EntityManager::renderAllEntities(Mat4x4 matCamera) {
    for (auto& batch : batches) {
        batch->render(matCamera);  // Render each batch
    }
    for (auto& entity : entities) {
        entity->render(matCamera);
    }
}
