#include "systems/PhysicsSystem.h"
#include <limits>

constexpr float epsilon = 1e-5f; // Small tolerance for floating-point errors

void PhysicsSystem::processEvent(const Event& event, float deltaTime) {

}

void PhysicsSystem::update(float deltaTime) {
    entities = registry.getEntitiesWith(requiredComponents);

    for (const auto& entity : entities) {
        Transform& transform = registry.getComponent<Transform>(entity);
        Physics& physics = registry.getComponent<Physics>(entity);

        if (physics.isStatic) continue;

        Vec3 p = transform.position;
        Vec3 pNext = p + (physics.velocity * deltaTime); 
        
        bool collision = false;
        std::vector<Entity> objects = registry.getEntitiesWith(TRANSFORM_MASK);
        for (auto obj : objects) {
            if (obj == entity) continue;
            
            Vec3 otherP = registry.getComponent<Transform>(obj).position;

            Vec3 AABBmin = otherP;
            Vec3 AABBmax = otherP + Vec3(1,1,1);    
            // Check if collision is going to happen
            if (PointInAABB(pNext, AABBmin, AABBmax)) {
                // Calculate collision point and direction
                float tEntry, tExit;
                Vec3 collisionNormal;
                
                if (rayDetectionAABB(p, (pNext - p), AABBmin, AABBmax, collisionNormal, tEntry, tExit)) {
                    // Get rebound vector and collision point
                    Vec3 collisionPoint = p + (pNext - p) * tEntry;                    

                    transform.position = collisionPoint + collisionNormal * epsilon;
                    physics.velocity = calculateRebound(physics.velocity, collisionNormal, 0.8f);
                    if (length(physics.velocity) < 0.2f) physics.isStatic = true;
                    collision = true;
                    break;
                }
            } 
        }
        if (!collision) {
            transform.position = pNext;
            physics.velocity = physics.velocity + (physics.acceleration * deltaTime);
            physics.acceleration.y = gravity;
        } 
    }

    entities.clear();
}

bool PhysicsSystem::overlapDetectionAABB(const Vec3& min1, const Vec3& max1, const Vec3& min2, const Vec3& max2) {
    return (min1.x <= max2.x && max1.x >= min2.x) && // Overlap on X
        (min1.y <= max2.y && max1.y >= min2.y) && // Overlap on Y
        (min1.z <= max2.z && max1.z >= min2.z);   // Overlap on Z
}

bool PhysicsSystem::PointInAABB(const Vec3& point, const Vec3& AABBmin, const Vec3& AABBmax) {
    return (point.x >= AABBmin.x && point.x <= AABBmax.x) &&
           (point.y >= AABBmin.y && point.y <= AABBmax.y) &&
           (point.z >= AABBmin.z && point.z <= AABBmax.z);
}


bool PhysicsSystem::rayDetectionAABB(const Vec3& point, const Vec3& direction, const Vec3& AABBmin, const Vec3& AABBmax, 
                                            Vec3& collisionNormal, float& tEntry, float& tExit) {
    
    float inf = std::numeric_limits<float>::infinity();
    float tMinX = -inf, tMinY = -inf, tMinZ = -inf;
    float tMaxX = inf, tMaxY = inf, tMaxZ = inf;

    if (std::abs(direction.x) > epsilon) {
        tMinX = (AABBmin.x - point.x) / direction.x;
        tMaxX = (AABBmax.x - point.x) / direction.x;
        if (direction.x < 0.0f) std::swap(tMinX, tMaxX); 
    } else if (point.x < AABBmin.x || point.x > AABBmax.x) {
        return false;
    }

    if (std::abs(direction.y) > epsilon) {
        tMinY = (AABBmin.y - point.y) / direction.y;
        tMaxY = (AABBmax.y - point.y) / direction.y;
        if (direction.y < 0.0f) std::swap(tMinY, tMaxY); 
    } else if (point.y < AABBmin.y || point.y > AABBmax.y) {
        return false;
    }

    if (std::abs(direction.z) > epsilon) {
        tMinZ = (AABBmin.z - point.z) / direction.z;
        tMaxZ = (AABBmax.z - point.z) / direction.z;
        if (direction.z < 0.0f) std::swap(tMinZ, tMaxZ); 
    } else if ( point.z < AABBmin.z || point.z > AABBmax.z) {
        return false;
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
    
    Vec3 rebound = velocity - norm * (2.0f * dot(velocity, norm));

    return rebound * clamp(bounceFactor, 0.1f, 0.9f);
}
