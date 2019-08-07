//
//  perlinMap.cpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 17.04.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#include "perlinMap.hpp"

PerlinMap::PerlinMap(unsigned int seed, unsigned int width, float triangleWidth, Shader *shader, const RenderData *data):
tex(""), model(1), translate(1), vertex(), texCoord(), position(vec3(0.0f)), data(data), shader(shader), width(width), noise(seed), triangleWidth(triangleWidth) {
    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);
    
    vertices = new vec3[6 * width * width * (1.0f / triangleWidth)];
    texCoords = new vec2[6 * width * width * (1.0f / triangleWidth)];
    
    float x = -(width / 2.0f), y = -(width / 2.0f);
    
    float freq = 4.0f, multiplier = 2.0f;
    int octaves = 10;
    
    for(long i = 0; i < width * width * 6 * (1.0f / triangleWidth); i += 6) {
        if(x >= width / 2.0f) {
            x = -(width / 2.0f);
            y += 1.0f;
        }
        
        vertices[i + 0] = vec3((x + 0.0f) * triangleWidth, noise.perl((x + 0.0f) * triangleWidth, (y + 0.0f) * triangleWidth, freq, octaves) * multiplier, (y + 0.0f) * triangleWidth);
        vertices[i + 1] = vec3((x + 1.0f) * triangleWidth, noise.perl((x + 1.0f) * triangleWidth, (y + 0.0f) * triangleWidth, freq, octaves) * multiplier, (y + 0.0f) * triangleWidth);
        vertices[i + 2] = vec3((x + 0.0f) * triangleWidth, noise.perl((x + 0.0f) * triangleWidth, (y + 1.0f) * triangleWidth, freq, octaves) * multiplier, (y + 1.0f) * triangleWidth);
        vertices[i + 3] = vec3((x + 1.0f) * triangleWidth, noise.perl((x + 1.0f) * triangleWidth, (y + 1.0f) * triangleWidth, freq, octaves) * multiplier, (y + 1.0f) * triangleWidth);
        vertices[i + 4] = vec3((x + 1.0f) * triangleWidth, noise.perl((x + 1.0f) * triangleWidth, (y + 0.0f) * triangleWidth, freq, octaves) * multiplier, (y + 0.0f) * triangleWidth);
        vertices[i + 5] = vec3((x + 0.0f) * triangleWidth, noise.perl((x + 0.0f) * triangleWidth, (y + 1.0f) * triangleWidth, freq, octaves) * multiplier, (y + 1.0f) * triangleWidth);
        
        texCoords[i + 0] = vec2(0.0f, 0.0f);
        texCoords[i + 1] = vec2(1.0f, 0.0f);
        texCoords[i + 2] = vec2(0.0f, 1.0f);
        texCoords[i + 3] = vec2(1.0f, 1.0f);
        texCoords[i + 4] = vec2(1.0f, 0.0f);
        texCoords[i + 5] = vec2(0.0f, 1.0f);
           
        x += 1.0f;
    }
    
    vertex.setData(vertices, sizeof(vec3) * 6 * width * width * (1.0f / triangleWidth), 0);
    texCoord.setData(texCoords, sizeof(vec2) * 6 * width * width * (1.0f / triangleWidth), 1);
    
    vertex.activate();
    texCoord.activate();
    
    glBindVertexArray(0);
}

PerlinMap::~PerlinMap() {
    glDeleteVertexArrays(1, &this->VAO);
    
    delete [] vertices;
    delete [] texCoords;
}

void PerlinMap::render() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex.getData());
    shader->sendInt(0, tex.getTextureName());
    
    glBindVertexArray(VAO);
    shader->sendMat4(*data->projection, "projection");
    shader->sendMat4(data->viewMat, "view");
    shader->sendMat4(model, "model");
    
    glDrawArrays(GL_TRIANGLES, 0, 6 * width * width * (1.0f / triangleWidth));
    glBindVertexArray(0);
}

void PerlinMap::setTexture(Texture tex) {
    this->tex = tex;
}

void PerlinMap::setPosition(vec3 position) {
    model = glm::translate(mat4(1), position);
    this->position = position;
}

vec3 PerlinMap::getPosition() {
    return position;
}

Shader* PerlinMap::getShaderPointer() {
    return shader;
}
