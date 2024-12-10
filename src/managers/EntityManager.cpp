#include "managers/EntityManager.h"
#include "managers/ComponentManager.h"
#include <typeindex>

Entity EntityManager::createEntity() {
    Entity entity;
    if (!availableIDs.empty()) {
        // Reuse an available ID
        entity = availableIDs.front();
        availableIDs.pop();
        // Increment version for reuse to avoid conflicts
        // Reset the lower 16 bits for version and set the new version
        entity.id = (entity.id & 0xFFFF0000) | ((entity.id + 1) & 0xFFFF); // Increment the version while keeping the index
    } else {
        // Assign a new entity ID
        entity.id = (nextEntity.id << 16); // Set index part to nextEntity.id, and version to 0
        nextEntity.id++; // Increment index for the next entity
    }
    
    activeEntities.insert(entity);
    return entity;
}


void EntityManager::destroyEntity(Entity entity) {
    // Reset componentMask
    entity.componentMask = 0;
    // Add the entity to the available IDs queue
    availableIDs.push(entity);
    // Remove from active entities
    activeEntities.erase(entity);
    // Delete entity from component manager
    ComponentManager::getInstance().entityDestroyed(entity);
}

bool EntityManager::isEntityAlive(Entity entity) {
    return activeEntities.find(entity) != activeEntities.end();
}

void EntityManager::removeComponentMask(Entity entity, uint32_t mask) {
    entity.componentMask = entity.componentMask & ~mask;
}

void EntityManager::addComponentMask(Entity entity, uint32_t mask) {
    entity.componentMask = entity.componentMask | mask;
}