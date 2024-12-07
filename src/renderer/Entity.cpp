#include "renderer/Entity.h"

InstanceData Entity::toInstanceData() {
    return { 0, this->texture->textureID, getMatWorld() };
}

void Entity::updateGPUBuffer() {

}

void Entity::render(Mat4x4 matCamera) {
    if (!shader || !shape) {
        std::cout << "Cannot render entity without shader and shape" << std::endl;
        return;
    } 
    this->shader->use(matCamera);
    if (texture) this->texture->bind(shader->programID);
    this->shape->draw(this->shader->programID, { getMatWorld() });
}

Mat4x4 Entity::getMatWorld() {
    Mat4x4 localMatWorld = MatrixRotation(this->direction.x, this->direction.y, this->direction.z) *
                        MatrixScaling(this->scale.x, this->scale.y, this->scale.z) *
                        MatrixTranslation(this->position.x, this->position.y, this->position.z);

    if (auto p = parent.lock()) {
        return p->getMatWorld() * localMatWorld;
    }

    return localMatWorld;
}