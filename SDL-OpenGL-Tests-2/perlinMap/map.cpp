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
    for(int x = -viewRange * 128; x < viewRange * 128; x += 128) {
        for(int y = -viewRange * 128; y < viewRange * 128; y += 128) {
            if(glm::distance(glm::vec2(x, y), glm::vec2(0.0f)) <= float(viewRange)) {
                requiredChunks.push_back(glm::vec2(x, y));
            }
        }
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
       
        if(!chunkLoaded) {*/
            chunks.push_back(std::make_unique<MapChunk>(MapChunk(&noise, shader, data, requiredChunks[i])));
            chunks[chunks.size() - 1]->setTexture(&texture);
//        }
    }
}

void Map::render() {
    for(int i = 0; i < chunks.size(); i++) {
        chunks[i]->render();
    }
}
