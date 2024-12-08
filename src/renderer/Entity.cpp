#include "renderer/Entity.h"

void Entity::updateGPUBuffer() {
    texture->loadToGPU();
}

void Entity::render(Mat4x4 matCamera) {
    if (!shader || !shape) {
        std::cout << "Cannot render entity without shader and shape" << std::endl;
        return;
    } 

    // Use program shader
    shader->use();

    glBindVertexArray(shape->getVAO());

    if (instanceDataDirty) {
        updateGPUBuffer();
        instanceDataDirty = false;
    }

    // Bind uniforms
    shader->bindMatrix(matCamera * getMatWorld());
    shader->bindTexture(texture->textureID);

    shape->drawInstances(1);

    glBindVertexArray(0);
}

Mat4x4 Entity::getMatWorld() {
    Mat4x4 localMatWorld = MatrixTranslation(this->position.x, this->position.y, this->position.z) *
                        MatrixScaling(this->scale.x, this->scale.y, this->scale.z) *
                        MatrixRotation(this->direction.x, this->direction.y, this->direction.z);
                        
    if (auto p = parent.lock()) {
        return p->getMatWorld() * localMatWorld;
    }

    return localMatWorld;
}