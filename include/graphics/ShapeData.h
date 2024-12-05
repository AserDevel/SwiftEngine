#ifndef SHAPEDATA_H
#define SHAPEDATA_H

#include <glad/glad.h>
#include <vector>
#include <iostream>
#include "linalg/linalg.h"

struct Vertex {
    Vec3 position;
    Vec2 texture;
    Vec3 color;

    Vertex(Vec3 pos = { 0,0,0 }, Vec2 tex = { 0,0 }, Vec3 col = { 1,1,1 }) : 
        position(pos), texture(tex), color(col) {} 
};

class ShapeData {
private:
    std::vector<Vertex> vertices;
    GLuint numVertices;
    std::vector<GLushort> indicies;
    GLuint numIndicies;

    GLuint VAO; // Vertex Array Object (stores attribute pointers)
    GLuint VBO; // Vertex Buffer Object (stores vertices)
    GLuint IBO; // Index Buffer Object (stores indicies)

public:
    ShapeData() {};
    ShapeData(const char* shapeFile);
    
    void setColor(float r, float g, float b);
    void loadFromOBJFile(std::string fileName, bool hasTexture = false);
    void sendToGPU();
    void draw(GLuint programID, Mat4x4 matFullTransform);
    void cleanup();
};

#endif