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

#define PI 3.14159f

// Window dimensions
int WINDOW_SIZE = 500;
const float ASPECT_RATIO = 16.0f / 9.0f;

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

int main(int argc, char* argv[]) {
    // Global instances
    SDL_Window* window = nullptr;
    SDL_GLContext glContext;

    // Initialize SDL and OpenGL
    if (!initializeWindow(&window, &glContext)) {
        std::cerr << "Error initializing window " << SDL_GetError() << std::endl;
        return -1;
    }    

    ResourceManager resourceManager;
    Camera camera(
        Vec3(0.0f, 0.0f, 0.0f),  // Position
        Vec3(0.0f, 1.0f, 0.0f),  // Up vector
        -3.14f / 2,                  // Yaw
        0.0f,                    // Pitch
        90.0f,                   // FOV
        1,                       // Aspect ratio
        0.1f,                    // Near plane
        100.0f                   // Far plane
    );
    float speed = 0.8f;

    // Load data to openGL
    std::shared_ptr<Shape> shape = resourceManager.getShape("lib/objects/cube.obj", true);
    std::shared_ptr<Texture> texture = resourceManager.getTexture("lib/textures/test.png");
    std::shared_ptr<Shader> shader = resourceManager.getShader("lib/shaders/textured3D.glsl");

    Entity cube(shader, texture, shape, { -2,0,8 });
    Entity cube1(shader, texture, shape, { 2,0,8});

    // Main loop
    bool isRunning = true;
    SDL_Event event;
    while (isRunning) {
        // Poll for events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            }
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                handleWindowResize(window);
            }
            //if (event.type = SDL_MOUSEMOTION) {
            //    camera.processMouseInput(event.motion.xrel, event.motion.yrel, 0.01);
            //}
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                case SDLK_UP:
                    camera.processMouseInput(0, -1);
                    break;
                case SDLK_DOWN:
                    camera.processMouseInput(0, 1);
                    break;
                case SDLK_RIGHT:
                    camera.processMouseInput(-1, 0);
                    break;
                case SDLK_LEFT:
                    camera.processMouseInput(1, 0);
                    break;
                case SDLK_a:
                    camera.processKeyboardInput("LEFT", 5, speed);
                    break;
                case SDLK_d:
                    camera.processKeyboardInput("RIGHT", 5, speed);                    
                    break;
                case SDLK_w:
                    camera.processKeyboardInput("FORWARD", 5, speed);
                    break;
                case SDLK_s:
                    camera.processKeyboardInput("BACKWARD", 5, speed);                    
                    break;
                default:
                    break;
                }
            }
        }
        // Clear the screen
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glDepthRange(0.1f, 10.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Mat4x4 matProj = camera.getMatProj();
        Mat4x4 matView = camera.getMatView();
        Mat4x4 matCamera = camera.getMatCamera();

        cube.render(matCamera);
        cube1.render(matCamera);

        // Swap buffers
        SDL_GL_SwapWindow(window);

        // FPS
        usleep(1000000 / 60); 
    }

    // Cleanup
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
