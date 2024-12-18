#include "systems/RenderSystem.h"
#include <algorithm>
#include "glad/glad.h"

const float globalAmbience = 0.1f;

void RenderSystem::processEvent(const Event& event, float deltaTime) {
    
}

void RenderSystem::update(float deltaTime) {
    // Clear the screen
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glDepthRange(0.1f, 10.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    entities = registry.getEntitiesWith(requiredComponents);
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
        auto& material = registry.getComponent<Material>(entity);
        shapeMap[material.shape].push_back(entity);
    }
    for (auto& [shape, entities] : shapeMap) {
        renderInstancesArray(entities);
    }
}


std::vector<LightData> RenderSystem::getLightSources(size_t amount) {
    // get all lights and sort them by distance to camera.
    auto entities = registry.getEntitiesWith(LIGHT_SOURCE_MASK | TRANSFORM_MASK);
    auto compare = [&](Entity a, Entity b) -> bool {
        Transform ta = registry.getComponent<Transform>(a);
        Transform tb = registry.getComponent<Transform>(b);
        return (length(camera->position - ta.position) > length(camera->position - tb.position));
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
    std::vector<LightData> lights;
    amount = std::min(amount, entities.size());
    for (size_t i = 0; i < amount; i++) {
        LightSource light = registry.getComponent<LightSource>(entities[i]);
        Vec3 position = registry.getComponent<Transform>(entities[i]).position;
        LightData data = {position, light.color, light.intensity, light.constant, light.linear, light.quadratic};
        lights.push_back(data);
    }
    return lights;
}


void RenderSystem::renderInstancesArray(std::vector<Entity> entities) {    
    // Generate instance data and texture array
    std::vector<InstanceData> instances;
    TextureArray textures;
    for (auto& entity : entities) {
        auto& material = registry.getComponent<Material>(entity);
        auto& transform = registry.getComponent<Transform>(entity);
        
        InstanceData newInstance;
        newInstance.matWorld = MatrixWorld(transform.position, transform.rotation, transform.scale);
        newInstance.reflectivity = material.reflectivity;
        newInstance.shininess = material.shininess;

        bool newTexture = true;
        for (int i = 0; i < textures.array.size(); i++) {
            if (material.texture == textures.array[i]) {
                newInstance.textureIndex = i;
                newTexture = false;
            }
        }

        if (newTexture) {
            newInstance.textureIndex = textures.array.size();
            textures.array.push_back(material.texture);
        }
        instances.push_back(newInstance);
    }

    auto& shape = registry.getComponent<Material>(entities[0]).shape;

    // Use the appropiate shader
    auto shader = resourceManager.getShader("lib/shaders/arrayVisual.glsl");
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
    shader->bindLights(getLightSources(8)); // bind 8 closest light sources to shader
    
    Vec3 sunDir = Vec3(0.6, -0.6, 0.6);
    Vec3 sunCol = Vec3(1.0, 0.9, 0.9);
    shader->bindVector(sunDir, "directionalLightDir");
    shader->bindVector(sunCol, "directionalLightColor");

    // Draw instances
    shape->drawInstancesArray(instances); 
}

void RenderSystem::renderInstancesAtlas(std::vector<Entity> entities) {
    
}