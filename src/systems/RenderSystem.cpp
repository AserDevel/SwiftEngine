#include "systems/RenderSystem.h"

void RenderSystem::filterEntities() {
    componentManager.forEach<Renderable>([&](Entity entity, Renderable& renderable) {
        if (match(entity)) entities.push_back(entity);
    });
}

void RenderSystem::processEvent(const Event& event, float deltaTime) {
    
}

void RenderSystem::update(float deltaTime) {
    // Clear the screen
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glDepthRange(0.1f, 10.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    filterEntities();
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
        if (entities.size() == 1) {
            renderSingle(entities[0]);
        } else {
            renderInstancesArray(entities);
        }
    }
}

void RenderSystem::renderSingle(Entity entity) {
    auto& renderable = componentManager.getComponent<Renderable>(entity);
    auto& transform = componentManager.getComponent<Transform>(entity);
    
    // Use the singleInstance shader
    auto shader = ResourceManager::getInstance().getShader("lib/shaders/single.glsl");
    shader->use();

    // Bind the VAO
    renderable.shape->bindVAO();

    // Bind texture and transoformation matrix
    shader->bindTexture(renderable.texture->textureID);
    shader->bindMatrix(camera->getMatCamera() * MatrixWorld(transform.position, transform.scale, transform.direction), "matFullTransform");

    // Draw instance
    renderable.shape->drawSingle();
}

void RenderSystem::renderInstancesArray(std::vector<Entity> entities) {    
    // Generate instance data and texture array
    std::vector<InstanceData> instances;
    TextureArray textures;

    for (auto& entity : entities) {
        auto& renderable = componentManager.getComponent<Renderable>(entity);
        auto& transform = componentManager.getComponent<Transform>(entity);
        
        InstanceData newInstance;
        newInstance.matWorld = MatrixWorld(transform.position, transform.scale, transform.direction);

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

    // Draw instances
    shape->drawInstancesArray(instances); 
}

void RenderSystem::renderInstancesAtlas(std::vector<Entity> entities) {
    
}