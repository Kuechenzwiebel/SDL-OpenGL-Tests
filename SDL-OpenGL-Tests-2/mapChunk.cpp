//
//  perlinMap.cpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 17.04.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#include "mapChunk.hpp"

MapChunk::MapChunk(unsigned int seed, unsigned int width, Shader *shader, const RenderData *data, glm::vec2 offset):
tex(nullptr), model(1), translate(1), vertex(), texCoord(), normal(), position(glm::vec3(0.0f)), data(data), shader(shader), width(width), noise(seed), vertices(pow(width, 2.0f) * 6), texCoords(pow(width, 2.0f) * 6), normals(pow(width, 2.0f) * 6) {
    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);
    
    float x = -(width / 2.0f) + offset.x, z = -(width / 2.0f) + offset.y;
    float u = 0.0f, v = 0.0f;
    int r = 0;
    
    noise.frequency = 15.0f;
    noise.multiplier = 3.5f;
    noise.octaves = 2;
    
    srand(seed);
    
    for(unsigned long i = 0; i < pow((width), 2.0f) * 6 ; i += 6) {
        if(x >= (width / 2.0f) + offset.x) {
            x = -(width / 2.0f) + offset.x;
            z += 1.0f;
        }
        
        vertices[i + 0] = glm::vec3(x + 0.0f, noise.noise(x + 0.0f, z + 0.0f), z + 0.0f);
        vertices[i + 1] = glm::vec3(x + 1.0f, noise.noise(x + 1.0f, z + 0.0f), z + 0.0f);
        vertices[i + 2] = glm::vec3(x + 0.0f, noise.noise(x + 0.0f, z + 1.0f), z + 1.0f);
        
        vertices[i + 3] = glm::vec3(x + 1.0f, noise.noise(x + 1.0f, z + 1.0f), z + 1.0f);
        vertices[i + 4] = glm::vec3(x + 1.0f, noise.noise(x + 1.0f, z + 0.0f), z + 0.0f);
        vertices[i + 5] = glm::vec3(x + 0.0f, noise.noise(x + 0.0f, z + 1.0f), z + 1.0f);
        
        
        normals[i + 0] = glm::triangleNormal(vertices[i + 0], vertices[i + 1], vertices[i + 2]) * -1.0f;
        normals[i + 1] = glm::triangleNormal(vertices[i + 0], vertices[i + 1], vertices[i + 2]) * -1.0f;
        normals[i + 2] = glm::triangleNormal(vertices[i + 0], vertices[i + 1], vertices[i + 2]) * -1.0f;
        
        normals[i + 3] = glm::triangleNormal(vertices[i + 5], vertices[i + 4], vertices[i + 3]) * -1.0f;
        normals[i + 4] = glm::triangleNormal(vertices[i + 5], vertices[i + 4], vertices[i + 3]) * -1.0f;
        normals[i + 5] = glm::triangleNormal(vertices[i + 5], vertices[i + 4], vertices[i + 3]) * -1.0f;
        
        
        r = prng(seed, int(x / 8), int(z / 4)) % 4;
        
        u = (r % 2) * 0.5f;
        v = (r / 2) * 0.5f;
        
        
        texCoords[i + 0] = glm::vec2(0.0f   +   fmod(fabs(x), 8.0f) / 8.0f, 0.0f  +   fmod(fabs(z), 4.0f) / 4.0f);
        texCoords[i + 1] = glm::vec2(0.125f +   fmod(fabs(x), 8.0f) / 8.0f, 0.0f  +   fmod(fabs(z), 4.0f) / 4.0f);
        texCoords[i + 2] = glm::vec2(0.0f   +   fmod(fabs(x), 8.0f) / 8.0f, 0.25f +   fmod(fabs(z), 4.0f) / 4.0f);
        
        texCoords[i + 3] = glm::vec2(0.125f +   fmod(fabs(x), 8.0f) / 8.0f, 0.25f +   fmod(fabs(z), 4.0f) / 4.0f);
        texCoords[i + 4] = glm::vec2(0.125f +   fmod(fabs(x), 8.0f) / 8.0f, 0.0f  +   fmod(fabs(z), 4.0f) / 4.0f);
        texCoords[i + 5] = glm::vec2(0.0f   +   fmod(fabs(x), 8.0f) / 8.0f, 0.25f +   fmod(fabs(z), 4.0f) / 4.0f);
        
        
        texCoords[i + 0] /= 2.0f;
        texCoords[i + 1] /= 2.0f;
        texCoords[i + 2] /= 2.0f;
        
        texCoords[i + 3] /= 2.0f;
        texCoords[i + 4] /= 2.0f;
        texCoords[i + 5] /= 2.0f;
        
        
        texCoords[i + 0] += glm::vec2(u, v);
        texCoords[i + 1] += glm::vec2(u, v);
        texCoords[i + 2] += glm::vec2(u, v);
        
        texCoords[i + 3] += glm::vec2(u, v);
        texCoords[i + 4] += glm::vec2(u, v);
        texCoords[i + 5] += glm::vec2(u, v);
        
        
        x += 1.0f;
    }
    
    
    vertices.shrink_to_fit();
    texCoords.shrink_to_fit();
    normals.shrink_to_fit();
    
    vertex.setData(vertices.data(), sizeof(glm::vec3) * pow(width, 2.0f) * 6, 0);
    texCoord.setData(texCoords.data(), sizeof(glm::vec2) * pow(width, 2.0f) * 6, 1);
    normal.setData(normals.data(), sizeof(glm::vec3) * pow(width, 2.0f) * 6, 2);
    
    vertex.activate();
    texCoord.activate();
    normal.activate();
    
    glBindVertexArray(0);
}

PerlinNoise* MapChunk::getNoise() {
    return &noise;
}

MapChunk::~MapChunk() {
    glDeleteVertexArrays(1, &this->VAO);
}

void MapChunk::render() {
    vertex.activate();
    texCoord.activate();
    normal.activate();
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex->getTextureID());
    shader->sendInt(0, tex->getTextureName());
    
    glBindVertexArray(VAO);
    shader->sendInt(0, "wireframe");
    
    shader->sendMat4(*data->projection, "projection");
    shader->sendMat4(data->viewMat, "view");
    shader->sendMat4(model, "model");
    
    glDrawArrays(GL_TRIANGLES, 0, pow(width, 2.0f) * 6);
    glBindVertexArray(0);
}

void MapChunk::setTexture(Texture *tex) {
    this->tex = tex;
}

void MapChunk::setPosition(glm::vec3 position) {
    model = glm::translate(glm::mat4(1), position);
    this->position = position;
}

glm::vec3 MapChunk::getPosition() {
    return position;
}

glm::vec3 MapChunk::getRealPosition() {
    return position;
}

Shader* MapChunk::getShaderPointer() {
    return shader;
}

void MapChunk::setRenderData(const RenderData *data) {
    this->data = data;
}
