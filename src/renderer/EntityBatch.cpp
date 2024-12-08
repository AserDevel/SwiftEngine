#include "renderer/EntityBatch.h"

GLuint EntityBatch::globalInstanceCounter = 0;

void EntityBatch::addInstance(std::shared_ptr<Texture> texture, Vec3 pos, Vec3 scale, Vec3 dir) {
    Mat4x4 matWorld = MatrixTranslation(pos.x, pos.y, pos.z) * MatrixScaling(scale.x , scale.y, scale.z) * 
                        MatrixRotation(dir.x, dir.y, dir.z);

    InstanceData newInstance = { 0, matWorld, 0 };

    // Check if the texture already exist in 'textures' and update the index
    bool newTextureIndex = true;
    for (int i = 0; i < textures.array.size(); i++) {
        if (texture == textures.array[i]) {
            newInstance.textureIndex = i;
            newTextureIndex = false;
            break;
        }
    }
    if (newTextureIndex) {
        newInstance.textureIndex = textures.array.size();
        textures.array.push_back(texture); 
    }

    // Update the instanceID, add the new instance to 'instances', and map it to the index
    newInstance.instanceID = ++globalInstanceCounter;
    instanceMap[newInstance.instanceID] = instances.size();
    instances.push_back(newInstance);
    instanceDataDirty = true;
}

void EntityBatch::destroyInstance(GLuint instanceID) {
    auto it = instanceMap.find(instanceID);
    if (it != instanceMap.end()) {
        size_t index = it->second;

        // Swap and pop removal
        instances[index] = instances.back();
        instanceMap[instances.back().instanceID] = index;
        instances.pop_back();

        instanceMap.erase(it);
        instanceDataDirty = true;
    } 
}

void EntityBatch::updateGPUBuffer() {    
    // load the texture array to gpu
    textures.loadToGPU();
    
    // Bind the instance buffer and upload instance data
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, instances.size() * sizeof(InstanceData), instances.data(), GL_STATIC_DRAW);

    // Enable the texture index as an instance attribute
    glEnableVertexAttribArray(3); 
    glVertexAttribIPointer(3, 1, GL_UNSIGNED_INT, sizeof(InstanceData), (void*)offsetof(InstanceData, textureIndex));
    glVertexAttribDivisor(3, 1); // Update per instance

    // Upload the transforms
    for (int i = 0; i < 4; i++) {
        glEnableVertexAttribArray(4 + i);  // Instance transform (one for each column of the matrix)
        glVertexAttribPointer(4 + i, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)(offsetof(InstanceData, matWorld) + sizeof(float) * i * 4));
        glVertexAttribDivisor(4 + i, 1);  // Tell OpenGL this attribute should be updated per instance
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void EntityBatch::render(Mat4x4 matCamera) {
    if (!shader || !shape) {
        std::cout << "Cannot render entity batch without shader and shape" << std::endl;
        return;
    }

    // Use the given shader program
    shader->use();

    // Bind the VAO for the shape
    glBindVertexArray(shape->getVAO());

    // Update the GPU buffer (if necessary)
    if (instanceDataDirty) {
        updateGPUBuffer();
        instanceDataDirty = false;        
    }

    // Bind uniforms
    shader->bindMatrix(matCamera);
    shader->bindTextureArray(textures.textureArrayID);

    // Draw the shape with instancing
    shape->drawInstances(instances.size());

    // Unbind the VAO
    glBindVertexArray(0);
}
