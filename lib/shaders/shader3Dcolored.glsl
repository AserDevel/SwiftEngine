#shader vertex
#version 460 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texture;
layout (location = 2) in vec3 color;

out vec3 vColor;

uniform mat4 matFullTransform;

void main() {
    gl_Position = matFullTransform * vec4(position, 1.0);
    vColor = color;
}


#shader fragment
#version 460 core

in vec3 vColor;

out vec4 fragColor;

void main() {
    fragColor = vec4(vColor, 1.0);
}