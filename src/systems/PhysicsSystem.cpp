#include "systems/PhysicsSystem.h"
#include <limits>

void PhysicsSystem::processEvent(const Event& event, float deltaTime) {

}

void PhysicsSystem::update(float deltaTime) {
    entities = registry.getEntitiesWith(requiredComponents);

    for (const auto& entity : entities) {
        Transform& transform = registry.getComponent<Transform>(entity);
        Physics& physics = registry.getComponent<Physics>(entity);

        Vec3 p = transform.position;
        Vec3 pNext = p + (physics.velocity * deltaTime);
        Vec3 vNext = physics.velocity + (physics.acceleration * deltaTime); 
        Vec3 dir = pNext - p;
        
        bool collision = false;
        std::vector<Entity> objects = registry.getEntitiesWith(TRANSFORM_MASK);
        for (auto obj : objects) {
            if (obj == entity) continue;
            Vec3 otherP = registry.getComponent<Transform>(obj).position;

            float tEntry, tExit;
            Vec3 collisionNormal;
            Vec3 AABBmin = otherP;
            Vec3 AABBmax = otherP + Vec3(1,1,1);
            
            // Check if collision is going to happen
            if (collisionDetectionAABB(p, dir, AABBmin, AABBmax, collisionNormal, tEntry, tExit)) {
                if (tEntry >= 0.0f && tEntry <= 1.0f) {  // Collision occurs within this step
                    Vec3 collisionPoint = p + dir * tEntry;                    

                    Vec3 rebound = calculateRebound(physics.velocity, collisionNormal, 0.5f);

                    collision = true;
                    physics.velocity = rebound;
                    transform.position = collisionPoint;
                }
                
            }
        }
        if (!collision) {
            transform.position = pNext;
            physics.velocity = vNext;
        } 
    }

    entities.clear();
}

bool PhysicsSystem::collisionDetectionAABB(const Vec3& point, const Vec3& direction, const Vec3& AABBmin, const Vec3& AABBmax, 
                                            Vec3& collisionNormal, float& tEntry, float& tExit) {
    
    constexpr float epsilon = 1e-5f; // Small tolerance for floating-point errors

    float inf = std::numeric_limits<float>::infinity();
    float tMinX = -inf, tMinY = -inf, tMinZ = -inf ;
    float tMaxX = inf, tMaxY = inf, tMaxZ = inf;
    
    if (std::abs(direction.x) > epsilon) {
        tMinX = (AABBmin.x - point.x) / direction.x;
        tMaxX = (AABBmax.x - point.x) / direction.x;
        if (direction.x < 0.0f) std::swap(tMinX, tMaxX); 
    }

    if (std::abs(direction.y) > epsilon) {
        tMinY = (AABBmin.y - point.y) / direction.y;
        tMaxY = (AABBmax.y - point.y) / direction.y;
        if (direction.y < 0.0f) std::swap(tMinY, tMaxY); 
    }

    if (std::abs(direction.x) > epsilon) {
        tMinZ = (AABBmin.z - point.z) / direction.z;
        tMaxZ = (AABBmax.z - point.z) / direction.z;
        
        if (direction.z < 0.0f) std::swap(tMinZ, tMaxZ); 
    }
    
    tEntry = std::max({tMinX, tMinY, tMinZ});
    tExit = std::min({tMaxX, tMaxY, tMaxZ});

    // calculate collision normal
    if (tEntry == tMinX) collisionNormal = (direction.x < 0) ? Vec3(1, 0, 0) : Vec3(-1, 0, 0);
    else if (tEntry == tMinY) collisionNormal = (direction.y < 0) ? Vec3(0, 1, 0) : Vec3(0, -1, 0);
    else if (tEntry == tMinZ) collisionNormal = (direction.z < 0) ? Vec3(0, 0, 1) : Vec3(0, 0, -1);

    return (tEntry <= tExit) && (tExit >= 0.0f);
}

Vec3 PhysicsSystem::calculateRebound(const Vec3& velocity, const Vec3& normal, float bounceFactor) {
    Vec3 norm = normalise(normal);
    
    float dotProduct = dot(velocity, norm);

    Vec3 rebound = velocity - norm * (2.0f * dotProduct);

    return rebound * bounceFactor;
}
