//
//  map.cpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 26.09.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#include "map.hpp"

Map::Map(PerlinNoise *noise, Shader *shader, const RenderData *data):
noise(noise), shader(shader), data(data), texture("resources/textures/stones.png") {
    
}

void Map::update(glm::vec3 cameraPosition) {
    requiredChunks.clear();
    
    for(int x = -viewRange; x <= viewRange; x += 128) {
        for(int y = -viewRange; y <= viewRange; y += 128) {
            if(glm::distance(glm::vec2(x, y), glm::vec2(0.0f)) <= float(viewRange)) {
                requiredChunks.push_back(glm::vec2(x, y) + round(cameraPosition.xz()));
            }
        }
    }
    
    bool chunkNotNeeded = true;
    
    for(int i = 0; i < chunks.size(); i++) {
        glm::vec2 position = chunks[i]->getPosition().xz();
        for(int j = 0; j < requiredChunks.size(); j++) {
            if(position == requiredChunks[j]) {
                printVec2(requiredChunks[j]);
                requiredChunks.erase(requiredChunks.begin() + j);
                chunkNotNeeded = false;
                break;
            }
        }
        
        if(chunkNotNeeded) {
            chunks.erase(chunks.begin() + i);
        }
    }
    
//    chunks.clear();
    
    
    for(int i = 0; i < requiredChunks.size(); i++) {
        std::cout << "Generating chunk ";
        printVec2(requiredChunks[i], false);
        std::cout << "\t\t";
        chunks.push_back(std::make_unique<MapChunk>(noise, shader, data, requiredChunks[i]));
        chunks[chunks.size() - 1]->setTexture(&texture);
        chunks[chunks.size() - 1]->setPosition(glm::vec3(0.0f, -2.0f, 0.0f));
    }
}

void Map::render() {
    for(int i = 0; i < chunks.size(); i++) {
        chunks[i]->render();
    }
}
