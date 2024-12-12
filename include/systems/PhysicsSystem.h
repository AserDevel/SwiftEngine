#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include "managers/ComponentManager.h"
#include "ISystem.h"

class PhysicsSystem : public ISystem {
private:
    uint32_t requiredComponents = TRANSFORM_MASK | PHYSICS_MASK;

    ComponentManager& componentManager = ComponentManager::getInstance();
    EntityManager& entityManager = EntityManager::getInstance();

    std::vector<Entity> entities;

    float gravity = 9.816;

public:
    PhysicsSystem() {}

    int getPriority() override { return 2; }

    void processEvent(const Event& event, float deltaTime) override;

    void update(float deltaTime) override;
};

#endif