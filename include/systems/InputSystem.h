#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H

#include "managers/ComponentManager.h"
#include "managers/ResourceManager.h"
#include "graphics/Camera.h"
#include "ISystem.h"

class InputSystem : public ISystem {
private:
    uint32_t requiredComponents = 0;
    
    SDL_Window* window;
    std::shared_ptr<Camera> camera;
    ComponentManager& componentManager = ComponentManager::getInstance();

    std::unordered_map<SDL_Scancode, bool> keyStates;

public:
    InputSystem(SDL_Window* window, std::shared_ptr<Camera> camera) 
        : window(window), camera(camera) {}

    int getPriority() override { return 1; }
    
    void processEvent(const Event& event, float deltaTime) override;

    void update(float deltaTime) override;

private:

    void processKeyStates(float deltaTime);
};

#endif