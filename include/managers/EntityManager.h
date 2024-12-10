#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <queue>
#include <unordered_set>
#include <inttypes.h>

struct Entity {
    uint32_t id; // version (bits 0-15) index (bits 16-31)
    uint32_t componentMask = 0;

    // Define equality comparison operator
    bool operator==(const Entity& other) const {
        return id == other.id;  // Compare the ids for equality
    }

    // Define inequality comparison operator
    bool operator!=(const Entity& other) const {
        return !(*this == other);  // Return the opposite of equality comparison
    }
};

// Custom hash function for Entity to use in unordered_set
namespace std {
    template <>
    struct hash<Entity> {
        std::size_t operator()(const Entity& entity) const {
            return std::hash<uint32_t>{}(entity.id);
        }
    };
}

class EntityManager {
    EntityManager() {}

    std::queue<Entity> availableIDs; // Reuse IDs when entities are destroyed
    std::unordered_set<Entity> activeEntities; // Track all active entities
    Entity nextEntity = { 0,0 };

public:
    static EntityManager& getInstance() {
        static EntityManager instance;
        return instance;
    }

    // Delete copy constructor and assignment operator
    EntityManager(const EntityManager&) = delete;
    EntityManager& operator=(const EntityManager&) = delete;

    Entity createEntity();
    void destroyEntity(Entity entity);
    bool isEntityAlive(Entity entity);

    void removeComponentMask(Entity entity, uint32_t mask);
    void addComponentMask(Entity entity, uint32_t mask);
};


#endif