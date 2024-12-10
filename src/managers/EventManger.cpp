#include "managers/EventManager.h"


void EventManager::convertSDLEvents(SDL_Event& sdlEvent) {
    while (SDL_PollEvent(&sdlEvent)) {
        switch (sdlEvent.type) {
        case SDL_QUIT:
            publish({EVENT_QUIT});
            break;
        case SDL_KEYDOWN:
            publish({EVENT_KEYDOWN, sdlEvent.key.keysym.scancode});
            break;
        case SDL_KEYUP:
            publish({EVENT_KEYUP, sdlEvent.key.keysym.scancode});
            break;
        case SDL_MOUSEMOTION:
            publish({EVENT_MOUSEMOTION, sdlEvent.motion});
            break;
        default:
            break;
        }   
    }
}

void EventManager::publish(const Event& event) {
    eventQueue.push(event);
}

bool EventManager::poll(Event& event) {
    if (eventQueue.empty()) return false;

    event = eventQueue.front();
    eventQueue.pop();
    return true;
}
