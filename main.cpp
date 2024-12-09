#include <iostream>
#include <fstream>
#include <time.h>
#include <unistd.h>
#include <limits>
#include <memory>
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include "linalg/linalg.h"
#include "graphics/Camera.h"
#include "managers/SystemManager.h"
#include "managers/ComponentManager.h"
#include "managers/ResourceManager.h"

// Window dimensions
int WINDOW_SIZE = 600;
float ASPECT_RATIO = 16.0f / 9.0f;

bool initializeWindow(SDL_Window** window, SDL_GLContext* context) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "Error initializing SDL: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    
    *window = SDL_CreateWindow(
        "GLTest",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_SIZE * ASPECT_RATIO,
        WINDOW_SIZE,
        SDL_WINDOW_OPENGL
    );

    if (!window) {
        std::cerr << "Error creating SDL window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    *context = SDL_GL_CreateContext(*window);
    if (!*context) {
        std::cerr << "Error creating OpenGL context: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(*window);
        SDL_Quit();
        return false;
    }

    // Load GLAD
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        SDL_GL_DeleteContext(*context);
        SDL_DestroyWindow(*window);
        SDL_Quit();
        return false;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    return true;
}

int main(int argc, char* argv[]) {
    // Global instances
    SDL_Window* window = nullptr;
    SDL_GLContext glContext;

    // Initialize SDL and OpenGL
    if (!initializeWindow(&window, &glContext)) {
        std::cerr << "Error initializing window " << SDL_GetError() << std::endl;
        return -1;
    }    

    SDL_ShowCursor(SDL_DISABLE);
    SDL_SetRelativeMouseMode(SDL_TRUE);

    std::shared_ptr camera = std::make_shared<Camera>(
        Vec3(0.0f, 0.0f, 0.0f),  // Position
        Vec3(0.0f, 1.0f, 0.0f),  // Up vector
        0,                       // Yaw
        0,                       // Pitch
        90.0f,                   // FOV
        1,                       // Aspect ratio
        0.1f,                    // Near plane
        1000.0f                  // Far plane
    );
    
    // init managers
    auto& EM = EntityManager::getInstance();
    auto& CM = ComponentManager::getInstance();
    auto& RM = ResourceManager::getInstance();
    auto& SM = SystemManager::getInstance();

    // Load data to openGL
    std::shared_ptr<Shape> cube = RM.getShape("lib/objects/cube.obj", true);
    std::shared_ptr<Texture> dirtTex = RM.getTexture("lib/textures/dirt.png");
    std::shared_ptr<Texture> cobblestoneTex = RM.getTexture("lib/textures/cobblestone.png");
    std::shared_ptr<Texture> stoneTex = RM.getTexture("lib/textures/stone.png");


    Renderable cobble = { cube, cobblestoneTex, 0.4, 8};
    Renderable stone = { cube, stoneTex, 1, 32 };
    Entity entity;
    Transform transform;

    for (int i = 0; i < 10; i++) {
        for (int j = -10; j < 10; j++) {
            entity = EM.createEntity();
            transform.position =  {i,-2,j};
            CM.addComponent<Renderable>(entity, cobble);
            CM.addComponent<Transform>(entity, transform);
        }   
    }

    for (int i = -10; i < 0; i++) {
        for (int j = -10; j < 10; j++) {
            entity = EM.createEntity();
            transform.position =  {i,-2,j};
            CM.addComponent<Renderable>(entity, stone);
            CM.addComponent<Transform>(entity, transform);
        }   
    }

    entity = EM.createEntity();
    transform.position = {5, 3, 0};
    transform.direction = {0, 1, 0};
    CM.addComponent<Renderable>(entity, cobble);
    CM.addComponent<Transform>(entity, transform);
    
    entity = EM.createEntity(); // White point light
    LightSource light = {{1,1,1}, {3,3,3}, 1.0f, 1.0f, 0.07f, 0.035f };
    CM.addComponent<LightSource>(entity, light);

    entity = EM.createEntity(); // red point light
    light.color = {1,0,0}; light.position = {-3, 3, -3};
    CM.addComponent<LightSource>(entity, light);

    entity = EM.createEntity(); // the sun
    light.color = { 0,0,1 }; light.position = { 5, 3, -5 };
    CM.addComponent<LightSource>(entity, light);

    float lastFrameTime = SDL_GetTicks() / 1000.0f;
    GameState currentState = NONE;
    while (currentState != QUIT) {
        float currentTime = SDL_GetTicks() / 1000.0f;
        float deltaTime = currentTime - lastFrameTime;
        lastFrameTime = currentTime;
        
        // Check for state switching and update systems accordingly
        if (currentState != SM.getState()) {
            currentState = SM.getState();
            SM.clearSystems();
            switch (currentState) {
            case QUIT:
                break;
            case INGAME:
                SM.registerSystem<InputSystem>(window, camera);
                SM.registerSystem<RenderSystem>(window, camera);
                //SM.registerSystem<PhysicsSystem>();
                break;
            case MAINMENU:
                SM.registerSystem<InputSystem>(window, camera);
                SM.registerSystem<RenderSystem>(window, camera);
                break;
            case PAUSEMENU:
                SM.registerSystem<InputSystem>(window, camera);
                SM.registerSystem<RenderSystem>(window, camera);
                break;
            default:
                break;
            }
        }

        // process and update this frame
        SM.processEvents(deltaTime);
        SM.update(deltaTime);
    }

    // Cleanup
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
