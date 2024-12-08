#include "Entity.h"
#include "unordered_map"

struct InstanceData {
    GLuint textureIndex; // Maps every instance to its texture
    Mat4x4 matWorld;
    GLuint instanceID; // Maps to instance index
};

// Use this for rendering multiple instances of the same entity type (shape and shader)
class EntityBatch {
    bool instanceDataDirty = false; // Set to true when instance data changes

    std::shared_ptr<Shader> shader;
    std::shared_ptr<Shape> shape;
    TextureArray textures;

    GLuint instanceVBO;
    std::vector<InstanceData> instances;  // Per-instance transformation data
    std::unordered_map<GLuint, size_t> instanceMap;  // Maps instanceID to index in instances
        
    static GLuint globalInstanceCounter;

public:
    EntityBatch(std::shared_ptr<Shader> shader, std::shared_ptr<Shape> shape)
        : shader(shader), shape(shape) { glGenBuffers(1, &instanceVBO); }
    
    ~EntityBatch() {
        if (instanceVBO) {
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glDeleteBuffers(1, &instanceVBO);
        }
    }

    void addInstance(std::shared_ptr<Texture> texture, Vec3 pos = { 0,0,0 }, Vec3 scale = { 1,1,1 }, Vec3 dir = { 0,0,0 });
    void destroyInstance(GLuint instanceID);

    // Update GPU buffer and set instanceDataDirty to false
    void updateGPUBuffer();

    void render(Mat4x4 matCamera);

    static GLuint getNextInstanceID() {
        return ++globalInstanceCounter;
    }
};