#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include <unordered_map>
#include <vector>
#include <functional>
#include <string>
#include <any>
#include <SDL2/SDL.h>
#include "EntityManager.h"
#include <memory>
#include <iostream>

enum EventType {
    EVENT_QUIT = 0,
    EVENT_KEYDOWN,
    EVENT_KEYUP,
    EVENT_MOUSEMOTION,
};

struct Event {
    EventType type;
    std::any data;
    Entity entity;  

    template <typename T>
    T castData() const {
        try {
            return std::any_cast<T>(data); // Cast the std::any to the desired type
        } catch (const std::bad_any_cast& e) {
            std::cerr << "Bad any_cast: " << e.what() << std::endl;
            throw; // Rethrow the exception or handle it appropriately
        }
    }
};


class EventManager {
private:
    EventManager() {}

    std::queue<Event> eventQueue;

public:
    static EventManager& getInstance() {
        static EventManager instance;
        return instance;
    }

    // Delete copy constructor and assignment operator
    EventManager(const EventManager&) = delete;
    EventManager& operator=(const EventManager&) = delete;

    // publish an event to the queue
    void publish(const Event& event);

    // poll an event from the queue
    bool poll(Event& event);

    // convertSDLevents to custom type Event
    void convertSDLEvents(SDL_Event& sdlEvent);
};

#endif
