#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H

#include "EntityManager.h"
#include "EventManager.h"
#include <SDL2/SDL.h>
#include <unordered_map>
#include <memory>
#include <err.h>
#include <stdexcept>
#include <algorithm>
#include "systems/RenderSystem.h"
#include "systems/InputSystem.h"
#include "systems/PhysicsSystem.h"

enum GameState {
    NONE,
    QUIT,
    MAINMENU,
    INGAME,
    PAUSEMENU,
};

class SystemManager {
private:
    SystemManager() {}

    EventManager& eventManager = EventManager::getInstance();

    GameState state = INGAME;

    std::vector<std::shared_ptr<ISystem>> systemExecutionOrder;
    std::unordered_map<std::string, std::shared_ptr<ISystem>> systems;

public:
    static SystemManager& getInstance() {
        static SystemManager instance;
        return instance;
    }

    // Delete copy constructor and assignment operator
    SystemManager(const SystemManager&) = delete;
    SystemManager& operator=(const SystemManager&) = delete;

    GameState getState() { return state; }

    void setState(GameState state) { this->state = state; }

    void clearSystems() {
        systems.clear();
        systemExecutionOrder.clear();
    }

    // Function to add a system
    template <typename T, typename... Args>
    std::shared_ptr<T> registerSystem(Args&&... args) {
        std::string typeName = typeid(T).name();  // Get the type name as string

        // Ensure the system isn't already registered
        if (systems.find(typeName) != systems.end()) {
            throw std::runtime_error("System already registered!");
        }

        // Create and store the system
        auto newSystem = std::make_shared<T>(std::forward<Args>(args)...);
        systems[typeName] = newSystem;
        systemExecutionOrder.push_back(newSystem);

        // Reorder systems to ensure the correct execution order
        reorderSystems();

        return newSystem;
    }

    // Function to remove a system
    template <typename T>
    void removeSystem() {
        std::string typeName = typeid(T).name();  // Get the type name as string

        // Ensure the system exists
        if (systems.find(typeName) == systems.end()) {
            throw std::runtime_error("System not found!");
        }

        // Remove from the map
        systems.erase(typeName);

        // Remove from the execution order
        systemExecutionOrder.erase(
            std::remove_if(systemExecutionOrder.begin(), systemExecutionOrder.end(),
                        [&](const std::shared_ptr<ISystem>& sys) { 
                            return std::string(typeid(*sys).name()) == typeName; 
                        }),
            systemExecutionOrder.end());
    }

    void processEvents(float deltaTime) {
        SDL_Event sdlEvent;
        eventManager.convertSDLEvents(sdlEvent);

        Event event;
        while (eventManager.poll(event)) {
            if (event.type == EVENT_QUIT) state = QUIT;
            for (const auto& system : systemExecutionOrder) {
                system->processEvent(event, deltaTime); 
            }
        }
    }

    // Function to update all systems in the correct order
    void update(float deltaTime) {
        for (auto system : systemExecutionOrder) {
            system->update(deltaTime);
        }
    }

private:
    // Helper to reorder systems after adding/removing
    void reorderSystems() {
        std::sort(systemExecutionOrder.begin(), systemExecutionOrder.end(),
                [](const std::shared_ptr<ISystem>& a, const std::shared_ptr<ISystem>& b) {
                    return a->getPriority() < b->getPriority();
                });
    }
};

#endif