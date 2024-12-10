#ifndef SHAPE_H
#define SHAPE_H

#include <glad/glad.h>
#include <vector>
#include <iostream>
#include "linalg/linalg.h"

struct Vertex {
    Vec3 position;
    Vec2 texCoord;
    Vec3 normal;

    Vertex(Vec3 pos = { 0,0,0 }, Vec2 tex = { 0,0 }, Vec3 norm = { 0,0,0 }) : 
        position(pos), texCoord(tex), normal(norm) {} 
};

struct InstanceData {
    GLuint textureIndex; // Maps every instance to its texture
    Mat4x4 matWorld;
};

class Shape {
private:
    std::vector<Vertex> vertices;
    std::vector<GLushort> indices;

    GLuint VAO; // Vertex Array Object (loading attribute pointers)
    GLuint VBO; // Vertex Buffer Object (loading vertices)
    GLuint IBO; // Index Buffer Object (loading indicies to vertices)
    GLuint instanceVBO; // Instance buffer used to load instance specific data

    void loadByIndexArray(std::string filename);
    void loadByVertexArray(std::string filename);
    void loadFromOBJFile(std::string filename, bool loadByIndices = true);
    void loadShapeToGPU();
    void cleanup();

public:
    ~Shape() { cleanup(); };
    Shape(const char* shapeFile, bool loadByIndices = true);
    
    void bindVAO() { glBindVertexArray(VAO); }
    
    void drawSingle();

    void drawInstancesArray(std::vector<InstanceData>& instances);

    void drawInstancesAtlas(std::vector<InstanceData>& instances);
};

#endif