//
//  map.hpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 26.09.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#ifndef map_hpp
#define map_hpp

#define GLM_FORCE_SWIZZLE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>

#include <stdio.h>
#include <vector>

#include "mapChunk.hpp"

const int viewRange = 128 * 2;

class Map {
public:
    Map(PerlinNoise *noise, Shader *shader, const RenderData *data);
    
    void update(glm::vec3 cameraPosition);
    void render();
    
private:
    Shader *shader;
    const RenderData *data;
    Texture texture;
    
    PerlinNoise *noise;
    
    std::vector<std::unique_ptr<MapChunk>> chunks;
    std::vector<glm::vec2> requiredChunks;
};

#endif /* map_hpp */
