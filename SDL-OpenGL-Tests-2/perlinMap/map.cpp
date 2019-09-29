//
//  map.cpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 26.09.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#include "map.hpp"

Map::Map(unsigned int seed, Shader *shader, const RenderData *data):
noise(seed), shader(shader), data(data), texture("resources/textures/stones.png") {
    noise.frequency = 15.0f;
    noise.multiplier = 3.5f;
    noise.octaves = 2;
}

void Map::update(glm::vec3 cameraPosition) {
    requiredChunks.clear();
    
    for(int x = -viewRange * 2; x < viewRange * 2; x += 128) {
        for(int y = -viewRange * 2; y < viewRange * 2; y += 128) {
            if(glm::distance(glm::vec2(x, y), glm::vec2(0.0f)) <= float(viewRange)) {
                requiredChunks.push_back(glm::vec2(x, y) + round(cameraPosition.xz()));
            }
        }
    }
    
    printf("Size: %lu\n", requiredChunks.size());
    
    for(int i = 0; i < requiredChunks.size(); i++) {
        printVec2(requiredChunks[i]);
    }
    
    
    chunks.clear();
    
    for(int i = 0; i < requiredChunks.size(); i++) {
        /*bool chunkLoaded = false;
        for(int j = 0; j < chunks.size(); j++) {
            if(chunks[j]->getPosition().xz() == requiredChunks[i]) {
                chunkLoaded = true;
                break;
            }
        }
       */
//        if(!chunkLoaded) {
            chunks.push_back(std::make_unique<MapChunk>(&noise, shader, data, requiredChunks[i]));
            chunks[chunks.size() - 1]->setTexture(&texture);
            chunks[chunks.size() - 1]->setPosition(glm::vec3(0.0f, -2.0f, 0.0f));
//        }
    }
}

void Map::render() {
    for(int i = 0; i < chunks.size(); i++) {
        chunks[i]->render();
    }
}
