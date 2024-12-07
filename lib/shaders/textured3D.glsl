#shader vertex
#version 460 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;
layout (location = 3) in mat4 matWorld;

out vec2 vTexCoord;

uniform mat4 matCamera;

void main() {
    gl_Position = matCamera * matWorld * vec4(position, 1.0);
    vTexCoord = texCoord;
}


#shader fragment
#version 460 core

in vec2 vTexCoord;

out vec4 fragColor;

uniform sampler2D textureData; 

void main() {
    fragColor = texture(textureData, vTexCoord);
}