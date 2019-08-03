//
//  dynamicArrayBuffer.cpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 08.03.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#include "dynamicArrayBuffer.hpp"

DynamicArrayBuffer::DynamicArrayBuffer(const GLfloat *data, int dataSize, int _shaderPos, arrayBufferType type):
shaderPos(_shaderPos), shaderSize(type), varType(GL_FLOAT) {
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    dataSet = true;
}

DynamicArrayBuffer::DynamicArrayBuffer(const GLint *data, int dataSize, int _shaderPos):
shaderPos(_shaderPos), shaderSize(1), varType(GL_INT) {
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    dataSet = true;
}

DynamicArrayBuffer::DynamicArrayBuffer(const vec3 *data, int dataSize, int _shaderPos):
shaderPos(_shaderPos), shaderSize(Vector3D), varType(GL_FLOAT) {
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    dataSet = true;
}

DynamicArrayBuffer::DynamicArrayBuffer(const vec2 *data, int dataSize, int _shaderPos):
shaderPos(_shaderPos), shaderSize(Vector2D), varType(GL_FLOAT) {
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    dataSet = true;
}

DynamicArrayBuffer::DynamicArrayBuffer() {
    
}

void DynamicArrayBuffer::setData(const vec3 *data, int dataSize, int _shaderPos) {
    varType = GL_FLOAT;
    shaderPos = _shaderPos;
    shaderSize = Vector3D;
    
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void DynamicArrayBuffer::setData(const vec2 *data, int dataSize, int _shaderPos) {
    varType = GL_FLOAT;
    shaderPos = _shaderPos;
    shaderSize = Vector2D;
    
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    dataSet = true;
}

void DynamicArrayBuffer::setData(const GLfloat *data, int dataSize, int _shaderPos, arrayBufferType type) {
    varType = GL_FLOAT;
    shaderPos = _shaderPos;
    shaderSize = type;
    
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    dataSet = true;
}

void DynamicArrayBuffer::changeData(const vec3 *data, int dataSize, int _shaderPos) {
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void DynamicArrayBuffer::changeData(const vec2 *data, int dataSize, int _shaderPos) {
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void DynamicArrayBuffer::changeData(const GLfloat *data, int dataSize, int _shaderPos, arrayBufferType type) {
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void DynamicArrayBuffer::activate() {
    if(dataSet == true) {
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glEnableVertexAttribArray(shaderPos);
        glVertexAttribPointer(shaderPos, shaderSize, varType, GL_FALSE, 0, (GLvoid *)0);
    }
    else {
        printf("Data for DynamicArrayBuffer not set!\n");
        return;
    }
}


DynamicArrayBuffer::~DynamicArrayBuffer() {
    glDeleteBuffers(1, &buffer);
}

