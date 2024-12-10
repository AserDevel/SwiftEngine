#ifndef COMPONENTMANAGER_H
#define COMPONENTMANAGER_H

#include <unordered_map>
#include <memory>
#include <cassert>
#include <typeindex>
#include <functional>
#include <iostream>
#include <stdexcept>
#include "EntityManager.h"
#include "components.h"

// Abstract base class for component arrays
class IComponentArray {
public:
    virtual ~IComponentArray() = default;
    virtual void entityDestroyed(Entity entity) = 0;
};

// Template for specific component arrays
template <typename T>
class ComponentArray : public IComponentArray {
    std::unordered_map<Entity, T> components;

public:
    void add(Entity entity, T component) {
        if (components.find(entity) != components.end()) {
            std::cerr << "Component already exists for this entity!\n";
            return;
        }
        components[entity] = component;
    }

    void remove(Entity entity) {
        if (components.find(entity) == components.end()) {
            std::cerr << "Trying to remove a non-existent component!\n";
            return;
        }
        components.erase(entity);
    }

    T& get(Entity entity) {
        auto it = components.find(entity);
        if (it == components.end()) {
            throw std::runtime_error("Trying to access a non-existent component!");
        }
        return components[entity];
    }

    void forEach(std::function<void(Entity, T&)> callback) {
        for (auto& [entity, component] : components) {
            callback(entity, component);
        }
    }

    void entityDestroyed(Entity entity) override {
        components.erase(entity);
    }
};

// ComponentManager class definition
class ComponentManager {
    std::unordered_map<std::type_index, std::unique_ptr<IComponentArray>> componentArrays;

    // Private constructor for Singleton
    ComponentManager() {}

public:
    // Singleton instance access
    static ComponentManager& getInstance() {
        static ComponentManager instance;
        return instance;
    }

    // Delete copy constructor and assignment operator
    ComponentManager(const ComponentManager&) = delete;
    ComponentManager& operator=(const ComponentManager&) = delete;

    // Add a component of type T to an entity
    template <typename T>
    void addComponent(Entity entity, T component) {
        auto& array = getComponentArray<T>();
        array.add(entity, component);
        EntityManager::getInstance().addComponentMask(entity, COMPONENT_MASKS.at(std::type_index(typeid(T))));
    }

    // Remove a component of type T from an entity
    template <typename T>
    void removeComponent(Entity entity) {
        auto& array = getComponentArray<T>();
        array.remove(entity);
        EntityManager::getInstance().removeComponentMask(entity, COMPONENT_MASKS.at(std::type_index(typeid(T))));
    }

    // Get a reference to a component of type T for an entity
    template <typename T>
    T& getComponent(Entity entity) {
        auto& array = getComponentArray<T>();
        return array.get(entity);
    }

    // Iterate over all components of type T
    template <typename T>
    void forEach(std::function<void(Entity, T&)> callback) {
        getComponentArray<T>().forEach(callback);
    }

    // Handle entity destruction by notifying all component arrays
    void entityDestroyed(Entity entity) {
        for (auto& [type, array] : componentArrays) {
            array->entityDestroyed(entity);
        }
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