#version 330 core

out vec4 color;

in vec3 FragPos;
in vec2 Uvs;
in vec3 Normals;

uniform sampler2D tex;

void main() {
    color = texture(tex, Uvs);
}
