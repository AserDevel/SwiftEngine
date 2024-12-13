#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include "managers/Registry.h"
#include "ISystem.h"

class PhysicsSystem : public ISystem {
    
    uint32_t requiredComponents = TRANSFORM_MASK | PHYSICS_MASK;

    Registry& registry = Registry::getInstance();

    std::vector<Entity> entities;

    float gravity = 9.816;

public:
    PhysicsSystem() {}

    int getPriority() override { return 2; }

    void processEvent(const Event& event, float deltaTime) override;

    void update(float deltaTime) override;

private:
    // Detects if and when a point and direction is going to hit an AABB
    bool collisionDetectionAABB(const Vec3& point, const Vec3& direction, const Vec3& AABBmin, const Vec3& AABBmax, 
                                Vec3& collisionNormal, float& tEntry, float& tExit); // return values

    Vec3 calculateRebound(const Vec3& velocity, const Vec3& normal, float bounceFactor);
};

#endif