#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uvs;

out vec2 Uvs;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec2 texOffset;
uniform vec2 windowSize;


void main() {
    gl_Position = projection * view * model * vec4(position, 1.0f);
    Uvs = vec2(uvs.x, 1.0f - uvs.y) + texOffset;
}

