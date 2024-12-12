#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include <bits/stdc++.h>
#include "graphics/Camera.h"
#include "ISystem.h"
#include "managers/ResourceManager.h"
#include "managers/ComponentManager.h"
#include "managers/EventManager.h"


class RenderSystem : public ISystem {
private:
    uint32_t requiredComponents = RENDERABLE_MASK | TRANSFORM_MASK;
    
    SDL_Window* window;
    std::shared_ptr<Camera> camera;
    
    ComponentManager& componentManager = ComponentManager::getInstance();
    ResourceManager& resourceManager = ResourceManager::getInstance();
    EntityManager& entityManager = EntityManager::getInstance();
    
    std::vector<Entity> entities;

public:
    RenderSystem(SDL_Window* window, std::shared_ptr<Camera> camera)
        : window(window), camera(camera) {}
        
    int getPriority() override { return 3; }

    void processEvent(const Event& event, float deltaTime) override;

    void update(float deltaTime) override;

private:
    // Distribute entities across different shaders and render them
    void renderEntities();

    // Gets the 'amount' most meaningful light sources
    std::vector<LightSource> getLightSources(size_t amount);

    // Render multiple instances of the same shape, using a texture array
    void renderInstancesArray(std::vector<Entity> entities);

    // Render multiple instances of the same shape, using a texture atlas
    void renderInstancesAtlas(std::vector<Entity> entities);
};


#endif