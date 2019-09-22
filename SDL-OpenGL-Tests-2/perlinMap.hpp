//
//  perlinMap.hpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 17.04.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#ifndef perlinMap_hpp
#define perlinMap_hpp

#include <stdio.h>
#include <vector>
#include <random>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/normal.hpp>
#include <glm/gtx/norm.hpp>

#include "perlin.hpp"
#include "utils.hpp"
#include "shader.hpp"
#include "object.hpp"
#include "arrayBuffers/arrayBuffer.hpp"
#include "texture.hpp"

struct PerlinMapInformation {
    PerlinNoise* noise;
    float multiplier;
    float freq;
    int octaves;
    unsigned int width;
};

class PerlinMap: public Object {
public:
    PerlinMap(unsigned int seed, unsigned int width, Shader *shader, const RenderData *data);
    ~PerlinMap();
    
    void setTexture(Texture *tex);
    void setPosition(glm::vec3 position);
    PerlinMapInformation getMapInfo();
    
    glm::vec3 getPosition();
    Shader *getShaderPointer();
    glm::vec3 getRealPosition();
    
    void render();
    
    
private:
    PerlinNoise noise;
    
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> normals;
    
    GLuint VAO;
    ArrayBuffer vertex, texCoord, normal;
    
    Texture *tex;
    
    Shader *shader;
    const RenderData *data;
    
    unsigned int width;
    float freq, multiplier;
    int octaves;
    
    glm::vec3 position;
    glm::mat4 translate, model;
};


#endif /* perlinMap_hpp */
