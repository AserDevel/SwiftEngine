#ifndef COMPONENTARRAY_H
#define COMPONENTARRAY_H

#include <unordered_map>
#include <memory>
#include <cassert>
#include <typeindex>
#include <functional>
#include <iostream>
#include <stdexcept>
#include "EntityManager.h"
#include "components.h"

using Entity = uint32_t;

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

    void entityDestroyed(Entity entity) override {
        components.erase(entity);
    }
};

#endif 