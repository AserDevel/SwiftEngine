#include "Entity.h"
#include "unordered_map"

// Use this for rendering multiple instances of the same entity type (shape and shader)
class EntityBatch {
    EntityCategory category;

    bool instanceDataDirty = false; // Set to true when instance data changes
    
    std::shared_ptr<Shader> shader;
    std::shared_ptr<Shape> shape;

    std::vector<InstanceData> instances;  // Per-instance transformation data
    std::unordered_map<GLuint, size_t> instanceMap;  // Maps instanceID to index in instances

public:
    void addInstance(const InstanceData& instance);
    void destroyInstance(GLuint instanceID);

    // Update GPU buffer and set instanceDataDirty to false
    void updateGPUBuffer();

    void renderEntities(Mat4x4 matViewProj);
};