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

class Shape {
private:
    std::vector<Vertex> vertices;
    std::vector<GLushort> indices;

    GLuint VAO; // Vertex Array Object (stores attribute pointers)
    GLuint VBO; // Vertex Buffer Object (stores vertices aka geometry data)
    GLuint IBO; // Index Buffer Object (stores indicies to vertices)

    void loadByIndexArray(std::string filename);
    void loadByVertexArray(std::string filename);
    void loadToGPU();
    void cleanup();

public:
    ~Shape() { cleanup(); };
    Shape(const char* shapeFile, bool loadByIndices = true);
    
    GLuint getVBO() { return VBO; }
    GLuint getVAO() { return VAO; }
    int getNumIndices() { return indices.size(); }
    int getNumVertices() { return vertices.size(); }

    void loadFromOBJFile(std::string filename, bool loadByIndices = true);
    void drawInstances(GLuint numInstances);
};

#endif