#ifndef OBJECT_H
#define OBJECT_H

#include "ShapeData.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include <memory>

class Object {
public:
    Vec3 position;
    Vec3 direction;
    std::shared_ptr<ShapeData> shape;
    std::shared_ptr<Texture> texture;
    std::shared_ptr<ShaderProgram> shader;
    
    Object(std::shared_ptr<ShapeData> shape, std::shared_ptr<Texture> texture, std::shared_ptr<ShaderProgram> shader, Vec3 pos, Vec3 dir);

    void render(Mat4x4 matProj, Mat4x4 matView);
};

#endif