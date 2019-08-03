//
//  instancingArrayBuffer.hpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 08.03.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#ifndef instancingArrayBuffer_hpp
#define instancingArrayBuffer_hpp

#include <stdio.h>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "arrayBuffer.hpp"

using namespace glm;

class InstancedArrayBuffer {
public:
    InstancedArrayBuffer(const vec3 *data, int _amount, int _shaderPos);
    InstancedArrayBuffer();
    ~InstancedArrayBuffer();
    
    void setData(const vec3 *data, int _amount, int _shaderPos);
    
    void activate();
    
private:
    bool dataSet = false;
    
    GLuint buffer;
    int amount;
    int shaderPos;
};

#endif /* instancingArrayBuffer_hpp */
