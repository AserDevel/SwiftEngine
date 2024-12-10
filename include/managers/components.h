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

// Define your bitmask constants
#define RENDERABLE_MASK (1 << 0)
#define TRANSFORM_MASK  (1 << 1)
#define PHYSICS_MASK    (1 << 2)
#define COLLIDER_MASK   (1 << 3)
#define CAMERA_MASK     (1 << 4)
#define INPUT_MASK      (1 << 5)
#define HEALTH_MASK     (1 << 6) 
#define AI_MASK         (1 << 7)
#define ANIMATION_MASK  (1 << 8)
#define AUDIO_MASK      (1 << 9)
#define LIGHT_MASK      (1 << 10)
#define NETWORK_MASK    (1 << 11)
#define PARTICLES_MASK  (1 << 12)
#define SCRIPT_MASK     (1 << 13)


struct Renderable {
    std::shared_ptr<Shape> shape;
    std::shared_ptr<Texture> texture;
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

struct Collider {
    enum class Shape { Sphere, Box, Capsule } shape; // Collision shape type (not the same as Shape in graphics)
    Vec3 size;        // Dimensions of the collider
    bool isTrigger;   // If true, the collider triggers events but does not block movement
};

// Maybe replacement for Camera class
//struct Camera {
//    float fov;          // Field of view
//    float nearPlane;    // Near clipping plane
//    float farPlane;     // Far clipping plane
//    float aspectRatio;  // Screen width/height ratio
//};

struct Input {
    bool isPlayerControlled = true;
    float movementSpeed; // Speed for movement
};

struct Health {
    int currentHealth;
    int maxHealth;
    int armor;  // Optional, for damage mitigation
};

// Need to implement Item
//struct Inventory {
//    std::vector<Item> items; // List of items
//    int capacity;            // Max number of items
//};

struct AI {
    enum class State { Idle, Patrol, Attack, Flee } state; // Current AI state
    std::vector<Vec3> waypoints; // Path for patrolling
};

struct Audio {
    std::string soundFile;  // File path for the sound
    float volume = 1.0f;    // Playback volume
    bool loop = false;      // Loop the sound?
};

struct Animation {
    std::string currentAnimation; // Name of the active animation
    float animationSpeed = 1.0f;  // Speed multiplier for the animation
};

struct Light {
    enum class Type { Point, Directional, Spot } type; // Light type
    Vec3 color = {1.0f, 1.0f, 1.0f}; // Light color
    float intensity = 1.0f;          // Brightness
};

struct Network {
    uint32_t networkID;  // Unique ID for syncing across clients
    bool isRemote = false; // True if the entity is controlled by another client
};

struct Particles {
    int maxParticles;       // Max number of particles
    float emissionRate;     // How many particles per second
    Vec3 velocity;          // Particle velocity
    float lifetime;         // Particle lifetime in seconds
};

struct Script {
    std::function<void(Entity, float)> onUpdate; // Custom update logic
};

const std::unordered_map<std::type_index, uint32_t> COMPONENT_MASKS = {
    { std::type_index(typeid(Renderable)), RENDERABLE_MASK },
    { std::type_index(typeid(Transform)), TRANSFORM_MASK },
    { std::type_index(typeid(Physics)), PHYSICS_MASK },
    { std::type_index(typeid(Collider)), COLLIDER_MASK },
    { std::type_index(typeid(Input)), INPUT_MASK },
    { std::type_index(typeid(Health)), HEALTH_MASK },
    { std::type_index(typeid(AI)), AI_MASK },
    { std::type_index(typeid(Animation)), ANIMATION_MASK },
    { std::type_index(typeid(Audio)), AUDIO_MASK },
    { std::type_index(typeid(Light)), LIGHT_MASK },
    { std::type_index(typeid(Network)), NETWORK_MASK },
    { std::type_index(typeid(Particles)), PARTICLES_MASK },
    { std::type_index(typeid(Script)), SCRIPT_MASK },
};


#endif