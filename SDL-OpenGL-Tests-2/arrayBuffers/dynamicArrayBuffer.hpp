//
//  dynamicArrayBuffer.hpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 08.03.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#ifndef dynamicArrayBuffer_hpp
#define dynamicArrayBuffer_hpp

#include <stdio.h>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "arrayBuffer.hpp"

using namespace glm;

class DynamicArrayBuffer {
public:
    DynamicArrayBuffer(const GLfloat *data, int dataSize, int _shaderPos, arrayBufferType type);
    DynamicArrayBuffer(const vec3 *data, int dataSize, int _shaderPos);
    DynamicArrayBuffer(const vec2 *data, int dataSize, int _shaderPos);
    DynamicArrayBuffer(const GLint *data, int dataSize, int _shaderPos);
    DynamicArrayBuffer();
    
    void setData(const vec3 *data, int dataSize, int _shaderPos);
    void setData(const vec2 *data, int dataSize, int _shaderPos);
    void setData(const GLfloat *data, int dataSize, int _shaderPos, arrayBufferType type);
    
    void changeData(const vec3 *data, int dataSize, int _shaderPos);
    void changeData(const vec2 *data, int dataSize, int _shaderPos);
    void changeData(const GLfloat *data, int dataSize, int _shaderPos, arrayBufferType type);
    
    ~DynamicArrayBuffer();
    
    void activate();
    
private:
    bool dataSet = false;
    
    GLenum varType;
    GLuint buffer;
    int shaderSize;
    int shaderPos;
};

#endif /* dynamicArrayBuffer_hpp */
