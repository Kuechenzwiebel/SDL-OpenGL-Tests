#version 330 core

layout (location = 0) in vec3 positions;
layout (location = 1) in vec2 uvs;
layout (location = 2) in vec3 normals;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float texOffset;

out vec3 SpacePos;
out vec2 Uvs;
out vec3 Normals;

uniform float t;
uniform float rx;
uniform float ry;

void main() {
    gl_Position = projection * view * model * vec4(positions, 1.0f);
    gl_Position.x += cos(t + rx) * 0.5f * 0.25f;
    gl_Position.y += -abs(sin(t + ry)) * 0.125f * 0.25f;
    SpacePos = (model * vec4(positions, 1.0f)).xyz;
    Uvs = vec2(uvs.x, 1.0f - uvs.y);
    Normals = mat3(inverse(model)) * normals;
}

