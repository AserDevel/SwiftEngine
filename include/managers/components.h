#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <unordered_map>
#include <typeindex>
#include <cstdint>
#include <memory>
#include "graphics/Shape.h"
#include "graphics/Shader.h"
#include "graphics/Texture.h"
#include "EntityManager.h"
#include "functional"


struct Renderable {
    std::shared_ptr<Shape> shape;
    std::shared_ptr<Texture> texture;
    float reflectivity;
    uint32_t shininess;
};

struct Transform {
    Vec3 position;
    Vec3 direction;
    Vec3 scale = {1.0f, 1.0f, 1.0f};
};

struct Physics {
    Vec3 velocity;      
    Vec3 acceleration;  
    float mass;        
    bool isStatic = false; // True for immovable objects
};

struct LightSource {
    Vec3 color;
    Vec3 position;
    float intensity;

    // Attenuation factors
    float constant = 1;
    float linear;
    float quadratic;
};

struct AI {
    enum class State { Idle, Patrol, Attack, Flee } state; // Current AI state
    std::vector<Vec3> waypoints; // Path for patrolling
};


// Define bitmask constants
#define RENDERABLE_MASK     (1 << 0)
#define TRANSFORM_MASK      (1 << 1)
#define PHYSICS_MASK        (1 << 2)
#define LIGHT_SOURCE_MASK   (1 << 3)
#define AI_MASK             (1 << 4)


const std::unordered_map<std::type_index, uint32_t> COMPONENT_MASKS = {
    { std::type_index(typeid(Renderable)), RENDERABLE_MASK },
    { std::type_index(typeid(Transform)), TRANSFORM_MASK },
    { std::type_index(typeid(Physics)), PHYSICS_MASK },
    { std::type_index(typeid(LightSource)), LIGHT_SOURCE_MASK },
    { std::type_index(typeid(AI)), AI_MASK },
};

#endif