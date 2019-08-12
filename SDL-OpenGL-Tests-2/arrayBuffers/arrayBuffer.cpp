//
//  arrayBuffer.cpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 08.03.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#include "arrayBuffer.hpp"

ArrayBuffer::ArrayBuffer(const GLfloat *data, int dataSize, int _shaderPos, arrayBufferType type):
shaderPos(_shaderPos), shaderSize(type), varType(GL_FLOAT) {
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    dataSet = true;
}

ArrayBuffer::ArrayBuffer(const GLint *data, int dataSize, int _shaderPos):
shaderPos(_shaderPos), shaderSize(1), varType(GL_INT) {
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    dataSet = true;
}

ArrayBuffer::ArrayBuffer(const glm::vec4 *data, int dataSize, int _shaderPos):
shaderPos(_shaderPos), shaderSize(Vector4D), varType(GL_FLOAT) {
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    dataSet = true;
}

ArrayBuffer::ArrayBuffer(const glm::vec3 *data, int dataSize, int _shaderPos):
shaderPos(_shaderPos), shaderSize(Vector3D), varType(GL_FLOAT) {
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    dataSet = true;
}

ArrayBuffer::ArrayBuffer(const glm::vec2 *data, int dataSize, int _shaderPos):
shaderPos(_shaderPos), shaderSize(Vector2D), varType(GL_FLOAT) {
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    dataSet = true;
}

ArrayBuffer::ArrayBuffer() {
    
}

void ArrayBuffer::setData(const glm::vec4 *data, int dataSize, int _shaderPos) {
    if(dataSet == false) {
        varType = GL_FLOAT;
        shaderPos = _shaderPos;
        shaderSize = Vector4D;
        
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        dataSet = true;
    }
    else {
        printf("%s", dataSetErrorMessage.c_str());
        return;
    }
}

void ArrayBuffer::setData(const glm::vec3 *data, int dataSize, int _shaderPos) {
    if(dataSet == false) {
        varType = GL_FLOAT;
        shaderPos = _shaderPos;
        shaderSize = Vector3D;
        
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        dataSet = true;
    }
    else {
        printf("%s", dataSetErrorMessage.c_str());
        return;
    }
}

void ArrayBuffer::setData(const glm::vec2 *data, int dataSize, int _shaderPos) {
    if(dataSet == false) {
        varType = GL_FLOAT;
        shaderPos = _shaderPos;
        shaderSize = Vector2D;
        
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        dataSet = true;
    }
    else {
        printf("%s", dataSetErrorMessage.c_str());
        return;
    }
}

void ArrayBuffer::setData(const GLfloat *data, int dataSize, int _shaderPos, arrayBufferType type) {
    if(dataSet == false) {
        varType = GL_FLOAT;
        shaderPos = _shaderPos;
        shaderSize = type;
        
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        dataSet = true;
    }
    else {
        printf("%s", dataSetErrorMessage.c_str());
    }
}


void ArrayBuffer::activate() {
    if(dataSet == true) {
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glEnableVertexAttribArray(shaderPos);
        glVertexAttribPointer(shaderPos, shaderSize, varType, GL_FALSE, 0, (GLvoid *)0);
    }
    else {
        printf("Data for ArrayBuffer not set!\n");
        return;
    }
}


ArrayBuffer::~ArrayBuffer() {
    glDeleteBuffers(1, &buffer);
}

ArrayBuffer::ArrayBuffer(const ArrayBuffer& other)  {
    dataSet = other.dataSet;
    varType = other.varType;
    buffer = other.buffer;
    shaderSize = other.shaderSize;
    shaderPos = other.shaderPos;
}

ArrayBuffer& ArrayBuffer::operator=(const ArrayBuffer& other) {
    if(this != &other) {
        dataSet = other.dataSet;
        varType = other.varType;
        buffer = other.buffer;
        shaderSize = other.shaderSize;
        shaderPos = other.shaderPos;
    }
    
    return *this;
}

