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

struct Material {
    std::shared_ptr<Shape> shape;
    std::shared_ptr<Texture> texture;
    float reflectivity = 0;
    int shininess = 0;

    Material(std::shared_ptr<Shape> shape = nullptr, std::shared_ptr<Texture> texture = nullptr, 
        float reflectivity = 0, int shininess = 0)
        : shape(std::move(shape)), 
        texture(std::move(texture)),
        reflectivity(clamp(reflectivity, 0, 1)), 
        shininess(pow(2, clamp(shininess, 0, 10))) {}
};

struct Transform {
    Vec3 position { 0,0,0 };
    Quat rotation = { 1,0,0,0 };
    Vec3 scale = { 1,1,1 };

    Transform(Vec3 pos = { 0,0,0 }, Quat rot = { 1,0,0,0 }, Vec3 scale = { 1,1,1 })
        : position(pos), rotation(rot), scale(scale) {}
};

struct Physics {
    Vec3 velocity;      
    Vec3 acceleration;  
    float mass;        
    bool isStatic = false; // True for immovable objects

    Physics(Vec3 velocity = 0, Vec3 acceleration = 0, float mass = 0, bool isStatic = false)
        : velocity(velocity), acceleration(acceleration), mass(std::max(0.0f, mass)), isStatic(isStatic) {}
};

struct LightSource {
    Vec3 color;
    float intensity;

    // Attenuation factors
    float constant = 1;
    float linear;
    float quadratic;

    LightSource(Vec3 color = { 1,1,1 }, float intensity = 1, float constant = 1, float linear = 0.14f, float quadratic = 0.07f)
        : color(color), intensity(intensity), constant(constant), linear(linear), quadratic(quadratic) {}
};

struct AI {
    enum class State { Idle, Patrol, Attack, Flee } state; // Current AI state
    std::vector<Vec3> waypoints; // Path for patrolling
};


// Define bitmask constants
#define MATERIAL_MASK       (1 << 0)
#define TRANSFORM_MASK      (1 << 1)
#define PHYSICS_MASK        (1 << 2)
#define LIGHT_SOURCE_MASK   (1 << 3)
#define AI_MASK             (1 << 4)


const std::unordered_map<std::type_index, uint32_t> COMPONENT_MASKS = {
    { std::type_index(typeid(Material)), MATERIAL_MASK },
    { std::type_index(typeid(Transform)), TRANSFORM_MASK },
    { std::type_index(typeid(Physics)), PHYSICS_MASK },
    { std::type_index(typeid(LightSource)), LIGHT_SOURCE_MASK },
    { std::type_index(typeid(AI)), AI_MASK },
};

#endif