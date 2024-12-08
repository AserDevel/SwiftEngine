#shader vertex
#version 460 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;
layout (location = 3) in uint texIndex;
layout (location = 4) in mat4 matWorld;

out vec2 vTexCoord;
flat out uint textureIndex;

uniform mat4 matTransform;

void main() {
    gl_Position = matTransform * matWorld * vec4(position, 1.0);
    vTexCoord = texCoord;
    textureIndex = texIndex;
}


#shader fragment
#version 460 core

in vec2 vTexCoord;
flat in uint textureIndex;

uniform sampler2DArray textureArray; 

out vec4 fragColor;

void main() {
    fragColor = texture(textureArray, vec3(vTexCoord, textureIndex));
}