//
//  arrayBuffer.hpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 08.03.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#ifndef arrayBuffer_hpp
#define arrayBuffer_hpp

#include <stdio.h>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <string>

using namespace glm;

enum arrayBufferType {
    Float = 1,
    Vector2D = 2,
    Vector3D = 3,
    Vector4D = 4
};

static std::string dataSetErrorMessage = "Data for ArrayBuffer is allready set! Use a dynamic ArrayBuffer if you need to change the data!\n";

class ArrayBuffer {
public:
    ArrayBuffer(const GLfloat *data, int dataSize, int _shaderPos, arrayBufferType type);
    ArrayBuffer(const vec4 *data, int dataSize, int _shaderPos);
    ArrayBuffer(const vec3 *data, int dataSize, int _shaderPos);
    ArrayBuffer(const vec2 *data, int dataSize, int _shaderPos);
    ArrayBuffer(const GLint *data, int dataSize, int _shaderPos);
    ArrayBuffer();
    
    ArrayBuffer(const ArrayBuffer& other);
    ArrayBuffer& operator=(const ArrayBuffer& other);
    
    void setData(const vec4 *data, int dataSize, int _shaderPos);
    void setData(const vec3 *data, int dataSize, int _shaderPos);
    void setData(const vec2 *data, int dataSize, int _shaderPos);
    void setData(const GLfloat *data, int dataSize, int _shaderPos, arrayBufferType type);
    
    ~ArrayBuffer();
    
    void activate();
    
private:
    bool dataSet = false;
    
    GLenum varType;
    GLuint buffer;
    int shaderSize;
    int shaderPos;
};

#endif /* arrayBuffer_hpp */
