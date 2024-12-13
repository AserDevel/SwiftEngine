#ifndef REGISTRY_H
#define REGISTRY_H

#include "EntityManager.h"
#include "ComponentArray.h"

// Registry class definition
class Registry {
    std::unordered_map<std::type_index, std::unique_ptr<IComponentArray>> componentArrays;

    EntityManager entityManager;

    // Private constructor for Singleton
    Registry() {}

public:
    // Singleton instance access
    static Registry& getInstance() {
        static Registry instance;
        return instance;
    }

    // Delete copy constructor and assignment operator
    Registry(const Registry&) = delete;
    Registry& operator=(const Registry&) = delete;

    Entity createEntity() {
        return entityManager.createEntity();
    }

    void destroyEntity(Entity entity) {
        // Remove from entitymanager
        entityManager.destroyEntity(entity);

        // Remove from component arrays
        for (auto& [type, array] : componentArrays) {
            array->entityDestroyed(entity);
        }
    }

    bool isAlive(Entity entity) {
        return entityManager.isEntityAlive(entity);
    }

    bool match(Entity entity, uint32_t componentMask) {
        return entityManager.match(entity, componentMask);
    }

    std::vector<Entity> getEntitiesWith(uint32_t componentMask) {
        return entityManager.getEntitiesByMask(componentMask);
    }
    
    // Add a component of type T to an entity
    template <typename T>
    void addComponent(Entity entity, T component) {
        auto& array = getComponentArray<T>();
        array.add(entity, component);
        entityManager.addComponentMask(entity, COMPONENT_MASKS.at(std::type_index(typeid(T))));
    }

    // Remove a component of type T from an entity
    template <typename T>
    void removeComponent(Entity entity) {
        auto& array = getComponentArray<T>();
        array.remove(entity);
        entityManager.removeComponentMask(entity, COMPONENT_MASKS.at(std::type_index(typeid(T))));
    }

    // Get a reference to a component of type T for an entity
    template <typename T>
    T& getComponent(Entity entity) {
        auto& array = getComponentArray<T>();
        return array.get(entity);
    }

private:
    // Get the specific component array for type T, creating it if necessary
    template <typename T>
    ComponentArray<T>& getComponentArray() {
        auto type = std::type_index(typeid(T));
        if (componentArrays.find(type) == componentArrays.end()) {
            componentArrays[type] = std::make_unique<ComponentArray<T>>();
        }
        return *static_cast<ComponentArray<T>*>(componentArrays[type].get());
    }
};

#endif