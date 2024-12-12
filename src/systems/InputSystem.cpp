#include "systems/InputSystem.h"

void InputSystem::processEvent(const Event& event, float deltaTime) {
    SDL_Scancode key;
    SDL_MouseMotionEvent motion;

    switch (event.type) {
    case EVENT_KEYDOWN:
        key = event.castData<SDL_Scancode>();
        keyStates[key] = true;
        if (key == SDL_SCANCODE_ESCAPE) {
            EventManager::getInstance().publish({EVENT_QUIT});
        }
        break;
    case EVENT_KEYUP:
        key = event.castData<SDL_Scancode>();
        keyStates[key] = false;
        break;
    case EVENT_MOUSEMOTION:
        motion = event.castData<SDL_MouseMotionEvent>();
        camera->processMouseInput(motion.xrel, motion.yrel, 0.02f);
    default:
        break;
    }
}

void InputSystem::update(float deltaTime) {
    processKeyStates(deltaTime);
}

void InputSystem::processKeyStates(float deltaTime) {
    const Uint8* state = SDL_GetKeyboardState(NULL);
    float speed = 5.0f;
    if (state[SDL_SCANCODE_LCTRL])
        speed *= 2;
    if (state[SDL_SCANCODE_W]) 
        camera->processKeyboardInput("FORWARD", deltaTime, speed);
    if (state[SDL_SCANCODE_S]) 
        camera->processKeyboardInput("BACKWARD", deltaTime, speed);
    if (state[SDL_SCANCODE_A]) 
        camera->processKeyboardInput("LEFT", deltaTime, speed);
    if (state[SDL_SCANCODE_D]) 
        camera->processKeyboardInput("RIGHT", deltaTime, speed);
    if (state[SDL_SCANCODE_SPACE]) 
        camera->processKeyboardInput("UP", deltaTime, speed);
    if (state[SDL_SCANCODE_LSHIFT])
        camera->processKeyboardInput("DOWN", deltaTime, speed);
}