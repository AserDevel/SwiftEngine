#shader vertex
#version 460 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texture;
layout (location = 2) in vec3 color;

out vec2 texCoord;

uniform mat4 matFullTransform;

void main() {
    gl_Position = matFullTransform * vec4(position, 1.0);
    texCoord = texture;
}


#shader fragment
#version 460 core

in vec2 texCoord;

out vec4 fragColor;

uniform sampler2D texture1; 

void main() {
    fragColor = texture(texture1, texCoord);
}