#version 330 core

out vec4 color;

in vec2 Uvs;

uniform sampler2D tex;

void main() {
    color = texture(tex, Uvs);
    if(color.a < 0.1f)
        color.a = 0.0f;
}
