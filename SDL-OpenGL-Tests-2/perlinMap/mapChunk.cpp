//
//  perlinMap.cpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 17.04.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#include "mapChunk.hpp"

static unsigned int randomMat[int(CHUNK_SIZE / SINGLE_STONE_TEX_WIDTH)][int(CHUNK_SIZE / SINGLE_STONE_TEX_HEIGHT)];

static void recalcRandomMat() {
    for(int x = 0; x < int(CHUNK_SIZE / SINGLE_STONE_TEX_WIDTH); x++) {
        for(int y = 0; y < int(CHUNK_SIZE / SINGLE_STONE_TEX_HEIGHT); y++) {
            randomMat[x][y] = rand() % 4;
        }
    }
}

MapChunk::MapChunk(PerlinNoise *noise, Shader *shader, const RenderData *data, glm::vec2 offset):
tex(nullptr), model(1), translate(1), vertex(), texCoord(), normal(), position(0.0f), offset(offset), data(data), shader(shader), noise(noise), vertices(CHUNK_ARRAY_SIZE), texCoords(CHUNK_ARRAY_SIZE), normals(CHUNK_ARRAY_SIZE) {
#ifdef PRINT_MAP_TIMES
    std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
#endif
    
//    printf("Random Mat size: %d %d\n", int(CHUNK_SIZE / SINGLE_STONE_TEX_WIDTH), int(CHUNK_SIZE / SINGLE_STONE_TEX_HEIGHT));
    
    recalcRandomMat();
    
    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);
    
    float x = -(CHUNK_SIZE / 2.0f) + offset.x, z = -(CHUNK_SIZE / 2.0f) + offset.y;
    float u = 0.0f, v = 0.0f;
    int r = 0;
    
    for(unsigned long i = 0; i < CHUNK_ARRAY_SIZE; i += 6) {
        if(x >= (CHUNK_SIZE / 2.0f) + offset.x) {
            x = -(CHUNK_SIZE / 2.0f) + offset.x;
            z += CHUNK_TRIANGLE_WIDTH;
        }
        
        float p1 = noise->octaveNoise(x + 0.0f, z + 0.0f), p2 = noise->octaveNoise(x + CHUNK_TRIANGLE_WIDTH, z + 0.0f), p3 = noise->octaveNoise(x + 0.0f, z + CHUNK_TRIANGLE_WIDTH), p4 = noise->octaveNoise(x + CHUNK_TRIANGLE_WIDTH, z + CHUNK_TRIANGLE_WIDTH);
        
        vertices[i + 0] = glm::vec3(x + 0.0f,                 p1, z + 0.0f                );
        vertices[i + 1] = glm::vec3(x + CHUNK_TRIANGLE_WIDTH, p2, z + 0.0f                );
        vertices[i + 2] = glm::vec3(x + 0.0f,                 p3, z + CHUNK_TRIANGLE_WIDTH);
        
        vertices[i + 3] = glm::vec3(x + CHUNK_TRIANGLE_WIDTH, p4, z + CHUNK_TRIANGLE_WIDTH);
        vertices[i + 4] = glm::vec3(x + CHUNK_TRIANGLE_WIDTH, p2, z + 0.0f                );
        vertices[i + 5] = glm::vec3(x + 0.0f,                 p3, z + CHUNK_TRIANGLE_WIDTH);
        
        
        glm::vec3 n1 = glm::triangleNormal(vertices[i + 0], vertices[i + 1], vertices[i + 2]) * -1.0f,
                  n2 = glm::triangleNormal(vertices[i + 5], vertices[i + 4], vertices[i + 3]) * -1.0f;
        
        normals[i + 0] = n1;
        normals[i + 1] = n1;
        normals[i + 2] = n1;
        
        normals[i + 3] = n2;
        normals[i + 4] = n2;
        normals[i + 5] = n2;
        
        
        
//        r = prng(noise->seed, int(x / 16), int(z / 8)) % 4;
        r =     randomMat[int((x - offset.x) / SINGLE_STONE_TEX_WIDTH + CHUNK_SIZE / SINGLE_STONE_TEX_WIDTH / 2)][int((z - offset.y) / SINGLE_STONE_TEX_HEIGHT + CHUNK_SIZE / SINGLE_STONE_TEX_HEIGHT / 2)];
//        printf("%d %d\n", int((x - offset.x) / SINGLE_STONE_TEX_WIDTH + CHUNK_SIZE / SINGLE_STONE_TEX_WIDTH / 2), int((z - offset.y) / SINGLE_STONE_TEX_HEIGHT + CHUNK_SIZE / SINGLE_STONE_TEX_HEIGHT / 2));
    
        u = (r % 2) * 0.5f;
        v = (r / 2) * 0.5f;
        
        texCoords[i + 0] = glm::vec2(0.0f /                 SINGLE_STONE_TEX_WIDTH + fmod(fabs(x), SINGLE_STONE_TEX_WIDTH) / SINGLE_STONE_TEX_WIDTH,   0.0f /                 SINGLE_STONE_TEX_HEIGHT + fmod(fabs(z), SINGLE_STONE_TEX_HEIGHT) / SINGLE_STONE_TEX_HEIGHT);
        texCoords[i + 1] = glm::vec2(CHUNK_TRIANGLE_WIDTH / SINGLE_STONE_TEX_WIDTH + fmod(fabs(x), SINGLE_STONE_TEX_WIDTH) / SINGLE_STONE_TEX_WIDTH,   0.0f /                 SINGLE_STONE_TEX_HEIGHT + fmod(fabs(z), SINGLE_STONE_TEX_HEIGHT) / SINGLE_STONE_TEX_HEIGHT);
        texCoords[i + 2] = glm::vec2(0.0f /                 SINGLE_STONE_TEX_WIDTH + fmod(fabs(x), SINGLE_STONE_TEX_WIDTH) / SINGLE_STONE_TEX_WIDTH,   CHUNK_TRIANGLE_WIDTH / SINGLE_STONE_TEX_HEIGHT + fmod(fabs(z), SINGLE_STONE_TEX_HEIGHT) / SINGLE_STONE_TEX_HEIGHT);
        
        texCoords[i + 3] = glm::vec2(CHUNK_TRIANGLE_WIDTH / SINGLE_STONE_TEX_WIDTH + fmod(fabs(x), SINGLE_STONE_TEX_WIDTH) / SINGLE_STONE_TEX_WIDTH,   CHUNK_TRIANGLE_WIDTH / SINGLE_STONE_TEX_HEIGHT + fmod(fabs(z), SINGLE_STONE_TEX_HEIGHT) / SINGLE_STONE_TEX_HEIGHT);
        texCoords[i + 4] = glm::vec2(CHUNK_TRIANGLE_WIDTH / SINGLE_STONE_TEX_WIDTH + fmod(fabs(x), SINGLE_STONE_TEX_WIDTH) / SINGLE_STONE_TEX_WIDTH,   0.0f /                 SINGLE_STONE_TEX_HEIGHT + fmod(fabs(z), SINGLE_STONE_TEX_HEIGHT) / SINGLE_STONE_TEX_HEIGHT);
        texCoords[i + 5] = glm::vec2(0.0f /                 SINGLE_STONE_TEX_WIDTH + fmod(fabs(x), SINGLE_STONE_TEX_WIDTH) / SINGLE_STONE_TEX_WIDTH,   CHUNK_TRIANGLE_WIDTH / SINGLE_STONE_TEX_HEIGHT + fmod(fabs(z), SINGLE_STONE_TEX_HEIGHT) / SINGLE_STONE_TEX_HEIGHT);
        
        
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
        
        
        
        x += CHUNK_TRIANGLE_WIDTH;
    }
    
    vertices.shrink_to_fit();
    texCoords.shrink_to_fit();
    normals.shrink_to_fit();
    
    vertex.setData(vertices.data(), sizeof(glm::vec3) * CHUNK_ARRAY_SIZE, 0);
    texCoord.setData(texCoords.data(), sizeof(glm::vec2) * CHUNK_ARRAY_SIZE, 1);
    normal.setData(normals.data(), sizeof(glm::vec3) * CHUNK_ARRAY_SIZE, 2);
    
    vertex.activate();
    texCoord.activate();
    normal.activate();
     
    glBindVertexArray(0);
    
#ifdef PRINT_MAP_TIMES
    std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
    std::cout << "Total time to generate chunk: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << "µs" << std::endl;
#endif
}

PerlinNoise* MapChunk::getNoise() {
    return noise;
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
    
    glDrawArrays(GL_TRIANGLES, 0, CHUNK_ARRAY_SIZE);
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
    return glm::vec3(offset.x, position.y, offset.y);
}

glm::vec3 MapChunk::getRealPosition() {
    return glm::vec3(INFINITY);
}

Shader* MapChunk::getShaderPointer() {
    return shader;
}

void MapChunk::setRenderData(const RenderData *data) {
    this->data = data;
}
