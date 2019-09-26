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

#include "perlinNoise.hpp"
#include "utils.hpp"
#include "shader.hpp"
#include "object.hpp"
#include "../arrayBuffers/arrayBuffer.hpp"
#include "texture.hpp"

class MapChunk: public Object {
public:
    MapChunk(unsigned int seed, Shader *shader, const RenderData *data, glm::vec2 offset);
    ~MapChunk();
    
    void setTexture(Texture *tex);
    void setPosition(glm::vec3 position);
    void setRenderData(const RenderData *data);
    
    PerlinNoise* getNoise();
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
    
    glm::vec3 position;
    glm::mat4 translate, model;
};


#endif /* perlinMap_hpp */
