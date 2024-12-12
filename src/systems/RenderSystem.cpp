#include "systems/RenderSystem.h"
#include <algorithm>

const float globalAmbience = 0.1f;

void RenderSystem::processEvent(const Event& event, float deltaTime) {
    
}

void RenderSystem::update(float deltaTime) {
    // Clear the screen
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glDepthRange(0.1f, 10.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    entities = entityManager.getEntitiesByMask(requiredComponents);
    renderEntities();

    // Swap buffers
    SDL_GL_SwapWindow(window);

    // Clear entity container for the next frame (will probably need optimisation in the future)
    entities.clear();
}

void RenderSystem::renderEntities() {
    // Map entities to shapes
    std::unordered_map<std::shared_ptr<Shape>, std::vector<Entity>> shapeMap;
    for (auto& entity : entities) {
        auto& renderable = componentManager.getComponent<Renderable>(entity);
        shapeMap[renderable.shape].push_back(entity);
    }
    for (auto& [shape, entities] : shapeMap) {
        renderInstancesArray(entities);
    }
}

std::vector<LightSource> RenderSystem::getLightSources(size_t amount) {
    // get all lights and sort them by distance to camera.
    auto entities = entityManager.getEntitiesByMask(LIGHT_SOURCE_MASK);
    auto compare = [&](Entity a, Entity b) -> bool {
        LightSource la = componentManager.getComponent<LightSource>(a);
        LightSource lb = componentManager.getComponent<LightSource>(b);
        return (length(camera->position - la.position) > length(camera->position - lb.position));
    };
    
    // Insertion sort
    for (int i = 1; i < entities.size(); i++) {
        Entity key = entities[i];
        int j = i - 1;

        while (j >= 0 && (compare(entities[j], key))) {
            entities[j + 1] = entities[j];
            j--;
        }
        entities[j + 1] = key;
    }

    // return the 'amount' first aka closest lights to the camera
    std::vector<LightSource> lights;
    amount = std::min(amount, entities.size());
    for (size_t i = 0; i < amount; i++) {
        lights.push_back(componentManager.getComponent<LightSource>(entities[i]));
    }
    return lights;
}


void RenderSystem::renderInstancesArray(std::vector<Entity> entities) {    
    // Generate instance data and texture array
    std::vector<InstanceData> instances;
    TextureArray textures;
    for (auto& entity : entities) {
        auto& renderable = componentManager.getComponent<Renderable>(entity);
        auto& transform = componentManager.getComponent<Transform>(entity);
        
        InstanceData newInstance;
        newInstance.matWorld = MatrixWorld(transform.position, transform.direction, transform.scale);
        newInstance.reflectivity = renderable.reflectivity;
        newInstance.shininess = renderable.shininess;

        bool newTexture = true;
        for (int i = 0; i < textures.array.size(); i++) {
            if (renderable.texture == textures.array[i]) {
                newInstance.textureIndex = i;
                newTexture = false;
            }
        }

        if (newTexture) {
            newInstance.textureIndex = textures.array.size();
            textures.array.push_back(renderable.texture);
        }
        instances.push_back(newInstance);
    }

    auto& shape = componentManager.getComponent<Renderable>(entities[0]).shape;

    // Use the appropiate shader
    auto shader = resourceManager.getShader("lib/shaders/textureArray.glsl");
    shader->use();

    // Bind the VAO
    shape->bindVAO(); 

    // Load texture array to GPU
    textures.loadTexturesToGPU();

    // Bind textures and camera matrix
    shader->bindTextureArray(textures.textureArrayID);
    shader->bindMatrix(camera->getMatCamera(), "matCamera");
    shader->bindFloat(globalAmbience, "globalAmbience");
    shader->bindVector(camera->position, "eyePos");
    shader->bindLights(getLightSources(8));

    // Draw instances
    shape->drawInstancesArray(instances); 
}

void RenderSystem::renderInstancesAtlas(std::vector<Entity> entities) {
    
}