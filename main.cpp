#include <iostream>
#include <fstream>
#include <time.h>
#include <unistd.h>
#include <limits>
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include "linalg/linalg.h"
#include "renderer/EntityManager.h"
#include "renderer/Camera.h"
#include "renderer/ResourceManager.h"

// Window dimensions
int WINDOW_SIZE = 500;
const float ASPECT_RATIO = 16.0f / 9.0f;
bool isRunning = true;
std::unordered_map<SDL_Scancode, bool> keyState;


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
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );

    if (!*window) {
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
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    return true;
}

void handleWindowResize(SDL_Window* window) {
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    if (h * ASPECT_RATIO > w ) {
        WINDOW_SIZE = h;
    } else {
        WINDOW_SIZE = w / ASPECT_RATIO;
    }
    SDL_SetWindowSize(window, WINDOW_SIZE * ASPECT_RATIO, WINDOW_SIZE);
    glViewport(0, 0, WINDOW_SIZE * ASPECT_RATIO, WINDOW_SIZE);
}

void processInput(Camera& camera, float deltaTime, float speed) {
    if (keyState[SDL_SCANCODE_W]) {
        camera.processKeyboardInput("FORWARD", deltaTime, speed);
    }
    if (keyState[SDL_SCANCODE_S]) {
        camera.processKeyboardInput("BACKWARD", deltaTime, speed);
    }
    if (keyState[SDL_SCANCODE_A]) {
        camera.processKeyboardInput("LEFT", deltaTime, speed);
    }
    if (keyState[SDL_SCANCODE_D]) {
        camera.processKeyboardInput("RIGHT", deltaTime, speed);
    }
    if (keyState[SDL_SCANCODE_UP]) {
        camera.processMouseInput(0, -1, 0.05f);
    }
    if (keyState[SDL_SCANCODE_DOWN]) {
        camera.processMouseInput(0, 1, 0.05f);
    }
    if (keyState[SDL_SCANCODE_LEFT]) {
        camera.processMouseInput(1, 0, 0.05f);
    }
    if (keyState[SDL_SCANCODE_RIGHT]) {
        camera.processMouseInput(-1, 0, 0.05f);
    }
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
    
    EntityManager entityManager;
    ResourceManager resourceManager;
    Camera camera(
        Vec3(0.0f, 0.0f, 0.0f),  // Position
        Vec3(0.0f, 1.0f, 0.0f),  // Up vector
        -PI / 2,                 // Yaw
        0.0f,                    // Pitch
        90.0f,                   // FOV
        1 / ASPECT_RATIO,        // Aspect ratio
        0.1f,                    // Near plane
        1000.0f                  // Far plane
    );
    float speed = 2.0f;
    
    // Load data to openGL
    std::shared_ptr<Shape> cube = resourceManager.getShape("lib/objects/cube.obj", true);
    std::shared_ptr<Texture> stone = resourceManager.getTexture("lib/textures/stone.png");
    std::shared_ptr<Texture> dirt = resourceManager.getTexture("lib/textures/dirt.png");
    std::shared_ptr<Texture> cobblestone = resourceManager.getTexture("lib/textures/cobblestone.png");
    std::shared_ptr<Shader> instancedShader = resourceManager.getShader("lib/shaders/tex3Dinstanced.glsl");
    std::shared_ptr<Shader> singleShader = resourceManager.getShader("lib/shaders/tex3Dsingle.glsl");

    auto cubes = entityManager.createEntityBatch(instancedShader, cube);
    entityManager.createEntity(singleShader, cube, cobblestone);

    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            if (j % 2) {
                cubes->addInstance(dirt, { i, -2, j });
            } else {
                cubes->addInstance(stone, { i, -2, j});
            }
        }
    }

    // Main loop
    
    float lastFrameTime = SDL_GetTicks() / 1000.0f;
    while (isRunning) {
        float currentTime = SDL_GetTicks() / 1000.0f;
        float deltaTime = currentTime - lastFrameTime;
        lastFrameTime = currentTime;
        // Poll for events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            } else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                handleWindowResize(window);
            } else if (event.type == SDL_KEYDOWN) {
                keyState[event.key.keysym.scancode] = true;
            } else if (event.type == SDL_KEYUP) {
                keyState[event.key.keysym.scancode] = false;
            } else if (event.type == SDL_MOUSEMOTION) {
                camera.processMouseInput(event.motion.xrel, event.motion.yrel, 0.01);
            }        
        }
        
        processInput(camera, deltaTime, speed);

        // Clear the screen
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glDepthRange(0.1f, 10.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Mat4x4 matCamera = camera.getMatCamera();

        auto start = SDL_GetPerformanceCounter();

        entityManager.renderAllEntities(matCamera);

        auto end = SDL_GetPerformanceCounter();
        std::cout << "Frame time (ms): " << (end - start) * 1000 / SDL_GetPerformanceFrequency() << std::endl;

        // Swap buffers
        SDL_GL_SwapWindow(window);
    }

    // Cleanup
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
