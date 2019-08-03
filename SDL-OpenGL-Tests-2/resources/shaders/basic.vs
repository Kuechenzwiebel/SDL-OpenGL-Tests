#version 330 core

layout (location = 0) in vec3 positions;
layout (location = 1) in vec2 uvs;
layout (location = 2) in vec3 normals;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float texOffset;

out vec3 FragPos;
out vec2 Uvs;
out vec3 Normals;

void main() {
    gl_Position = projection * view * model * vec4(positions, 1.0f);
    
    FragPos = (model * vec4(positions, 1.0f)).xyz;
    Uvs = vec2(uvs.x, 1.0f - uvs.y);
    Normals = mat3(transpose(inverse(model))) * normals;
}

