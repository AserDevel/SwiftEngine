#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <queue>
#include <unordered_set>
#include <inttypes.h>
#include <unordered_map>
#include "components.h"

using Entity = uint32_t;

class EntityManager {
    std::queue<Entity> availableIDs; // Reuse IDs when entities are destroyed
    std::unordered_set<Entity> activeEntities; // Track all active entities
    Entity nextEntity = 0;

    std::unordered_map<Entity, uint32_t> entityMasks;

public:

    EntityManager() {}

    Entity createEntity();

    void destroyEntity(Entity entity);

    bool isEntityAlive(Entity entity);

    bool match(Entity entity, u_int32_t mask);

    void removeComponentMask(Entity entity, uint32_t mask);
    
    void addComponentMask(Entity entity, uint32_t mask);

    std::vector<Entity> getEntitiesByMask(uint32_t mask);
};

#endif