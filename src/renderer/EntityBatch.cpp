#include "renderer/EntityBatch.h"


void EntityBatch::addInstance(const InstanceData& instance) {
    
}
void EntityBatch::destroyInstance(GLuint instanceID) {

}

void EntityBatch::updateGPUBuffer() {

}

void EntityBatch::renderEntities(Mat4x4 matCamera) {
    if (!shader || !shape) {
        std::cout << "Cannot render entity batch without shader and shape" << std::endl;
    }
    this->shader->use(matCamera);
    for (auto i : instances) {
        
    }
}