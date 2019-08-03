#version 330 core

out vec4 color;

in vec2 Uvs;

uniform sampler2D debug;

float near = 0.1;
float far  = 100.0;

float linearizeDepth(float depth) {
    float z = depth * 2.0 - 1.0; // back to NDC
    return (2.0 * near * far) / (far + near - z * (far - near));
}

float offset = 1.0 / 300.0f;

vec2 offsets[9] = vec2[] (
    vec2(-offset, offset),
    vec2(0.0f, offset),
    vec2(offset, offset),
    vec2(-offset, 0.0f),
    vec2(0.0f, 0.0f),
    vec2(offset, 0.0f),
    vec2(-offset, -offset),
    vec2(0.0f, -offset),
    vec2(offset, -offset)
);

float kernel[9] = float[] (
    1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f,
    2.0f / 16.0f, 4.0f / 16.0f, 2.0f / 16.0f,
    1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f
);

void main() {
    color = vec4(0.0f);
    for(int i = 0; i < 9; i++) {
        color += texture(debug, Uvs.st + offsets[i]) * kernel[i];
    }
}
