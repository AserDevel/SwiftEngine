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
    uint32_t setRequiredComponents = RENDERABLE_MASK | TRANSFORM_MASK;
    
    SDL_Window* window;
    std::shared_ptr<Camera> camera;
    
    ComponentManager& componentManager = ComponentManager::getInstance();
    ResourceManager& resourceManager = ResourceManager::getInstance();
    
    std::vector<Entity> entities;

public:
    RenderSystem(SDL_Window* window, std::shared_ptr<Camera> camera)
        : window(window), camera(camera) {}
        
    int getPriority() override { return 3; }

    void processEvent(const Event& event, float deltaTime) override;

    void update(float deltaTime) override;

private:
    // Filter all entities with required components
    void filterEntities();

    // Distribute entities across different shaders and render them
    void renderEntities();

    // Render multiple instances of the same shape, using a texture array
    void renderInstancesArray(std::vector<Entity> entities);

    // Render multiple instances of the same shape, using a texture atlas
    void renderInstancesAtlas(std::vector<Entity> entities);

    // Render a single instance
    void renderSingle(Entity entity);
};


#endif