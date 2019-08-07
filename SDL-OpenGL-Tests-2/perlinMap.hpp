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
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/normal.hpp>

#include "perlin.hpp"
#include "utils.hpp"
#include "shader.hpp"
#include "object.hpp"
#include "arrayBuffers/arrayBuffer.hpp"
#include "texture.hpp"

using namespace glm;

class PerlinMap: public Object {
public:
    PerlinMap(unsigned int seed, unsigned int width, Shader *shader, const RenderData *data);
    ~PerlinMap();
    
    void setTexture(Texture tex);
    void setPosition(vec3 position);
    
    vec3 getPosition();
    Shader *getShaderPointer();
    
    void render();
    
private:
    PerlinNoise noise;
    
    vec3 *vertices;
    vec2 *texCoords;
    
    GLuint VAO;
    ArrayBuffer vertex;
    ArrayBuffer texCoord;
    
    Texture tex;
    
    Shader *shader;
    const RenderData *data;
    
    unsigned int width;
    vec3 position;
    mat4 translate, model;
};


#endif /* perlinMap_hpp */
