#include "systems/PhysicsSystem.h"

void PhysicsSystem::processEvent(const Event& event, float deltaTime) {

}

void PhysicsSystem::update(float deltaTime) {
    entities = entityManager.getEntitiesByMask(requiredComponents);

    for (const auto& entity : entities) {
        Transform& transform = componentManager.getComponent<Transform>(entity);
        Physics& physics = componentManager.getComponent<Physics>(entity);

        Vec3 p = transform.position;
        Vec3 pNext = p + (physics.velocity * deltaTime);
        physics.velocity += physics.acceleration * deltaTime; 
        physics.acceleration.y -= gravity * deltaTime;
        
        std::vector<Entity> objects = entityManager.getEntitiesByMask(TRANSFORM_MASK);
        for (const auto& obj : objects) {
            Transform objTran = componentManager.getComponent<Transform>(obj);
            Vec3 p2 = objTran.position;
            if ((p.x < p2.x && pNext.x > p2.x) || (p.x > p2.x && pNext.x < p2.x)) {
                pNext.x = p.x;
            }
            if ((p.y + 1 <= p2.y && pNext.y + 1 >= p2.y) || (p.y >= p2.y + 1 && pNext.y <= p2.y + 1)) {
                pNext.y = p2.y + 1;
            }
            if ((p.z < p2.z && pNext.z > p2.z) || (p.z > p2.z && pNext.z < p2.z)) {
                pNext.z = p.z;
            }
        }
        transform.position = pNext;
    }
    entities.clear();
}