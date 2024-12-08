#shader vertex
#version 460 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

out vec2 TexCoord;

uniform mat4 matTransform;

void main() {
    gl_Position = matTransform * vec4(position, 1.0);
    TexCoord = texCoord;
}


#shader fragment
#version 460 core

in vec2 TexCoord;

uniform sampler2D textureData; 

out vec4 fragColor;

void main() {
    fragColor = texture(textureData, TexCoord);
}