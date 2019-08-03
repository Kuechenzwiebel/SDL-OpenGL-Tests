#version 330 core

layout (location = 0) in vec3 position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 Colors;
out vec3 Uvs;

void main() {
    vec4 pos = projection * view * vec4(position, 1.0f);
    Uvs = position;
    gl_Position = pos.xyww;
}
