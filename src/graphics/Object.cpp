#include "graphics/Object.h"
#include <fstream>
#include <SDL2/SDL_image.h>
#include "glad/glad.h"

Object::Object(std::shared_ptr<ShapeData> shape, std::shared_ptr<Texture> texture, std::shared_ptr<ShaderProgram> shader, Vec3 pos, Vec3 dir)
	: shape(shape), texture(texture), shader(shader), position(pos), direction(dir) {}

void Object::render(Mat4x4 matProj, Mat4x4 matView) {
	Mat4x4 matWorld = MatrixRotation(direction.x, direction.y, direction.z) * 
					MatrixTranslation(position.x, position.y, position.z);

	Mat4x4 matFullTransformation = matWorld * matView * matProj;

	shader->use();
	texture->bind(shader->programID);
	shape->draw(shader->programID, matFullTransformation);
}
