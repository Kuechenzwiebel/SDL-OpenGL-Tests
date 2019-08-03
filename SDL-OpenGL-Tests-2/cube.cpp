//
//  cube.cpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 16.04.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#include "cube.hpp"

vec3 cubeVertices[] = {
    vec3(-0.5f, -0.5f, -0.5f),
    vec3(0.5f, -0.5f, -0.5f),
    vec3(0.5f,  0.5f, -0.5f),
    vec3(0.5f,  0.5f, -0.5f),
    vec3(-0.5f,  0.5f, -0.5f),
    vec3(-0.5f, -0.5f, -0.5f),
    
    vec3(-0.5f, -0.5f,  0.5f),
    vec3(0.5f, -0.5f,  0.5f),
    vec3(0.5f,  0.5f,  0.5f),
    vec3(0.5f,  0.5f,  0.5f),
    vec3(-0.5f,  0.5f,  0.5f),
    vec3(-0.5f, -0.5f,  0.5f),
    
    vec3(-0.5f,  0.5f,  0.5f),
    vec3(-0.5f,  0.5f, -0.5f),
    vec3(-0.5f, -0.5f, -0.5f),
    vec3(-0.5f, -0.5f, -0.5f),
    vec3(-0.5f, -0.5f,  0.5f),
    vec3(-0.5f,  0.5f,  0.5f),
    
    vec3(0.5f,  0.5f,  0.5f),
    vec3(0.5f,  0.5f, -0.5f),
    vec3(0.5f, -0.5f, -0.5f),
    vec3(0.5f, -0.5f, -0.5f),
    vec3(0.5f, -0.5f,  0.5f),
    vec3(0.5f,  0.5f,  0.5f),
    
    vec3(-0.5f, -0.5f, -0.5f),
    vec3(0.5f, -0.5f, -0.5f),
    vec3(0.5f, -0.5f,  0.5f),
    vec3(0.5f, -0.5f,  0.5f),
    vec3(-0.5f, -0.5f,  0.5f),
    vec3(-0.5f, -0.5f, -0.5f),
    
    vec3(-0.5f,  0.5f, -0.5f),
    vec3(0.5f,  0.5f, -0.5f),
    vec3(0.5f,  0.5f,  0.5f),
    vec3(0.5f,  0.5f,  0.5f),
    vec3(-0.5f,  0.5f,  0.5f),
    vec3(-0.5f,  0.5f, -0.5f)
};

vec3 cubeNormals[] = {
    triangleNormal(vec3(-0.5f, -0.5f, -0.5f), vec3(0.5f, -0.5f, -0.5f), vec3(0.5f,  0.5f, -0.5f)),
    triangleNormal(vec3(-0.5f, -0.5f, -0.5f), vec3(0.5f, -0.5f, -0.5f), vec3(0.5f,  0.5f, -0.5f)),
    triangleNormal(vec3(-0.5f, -0.5f, -0.5f), vec3(0.5f, -0.5f, -0.5f), vec3(0.5f,  0.5f, -0.5f)),
    triangleNormal(vec3(0.5f,  0.5f, -0.5f), vec3(-0.5f,  0.5f, -0.5f), vec3(-0.5f, -0.5f, -0.5f)),
    triangleNormal(vec3(0.5f,  0.5f, -0.5f), vec3(-0.5f,  0.5f, -0.5f), vec3(-0.5f, -0.5f, -0.5f)),
    triangleNormal(vec3(0.5f,  0.5f, -0.5f), vec3(-0.5f,  0.5f, -0.5f), vec3(-0.5f, -0.5f, -0.5f)),
    
    triangleNormal(vec3(-0.5f, -0.5f,  0.5f), vec3(0.5f, -0.5f,  0.5f), vec3(0.5f,  0.5f,  0.5f)),
    triangleNormal(vec3(-0.5f, -0.5f,  0.5f), vec3(0.5f, -0.5f,  0.5f), vec3(0.5f,  0.5f,  0.5f)),
    triangleNormal(vec3(-0.5f, -0.5f,  0.5f), vec3(0.5f, -0.5f,  0.5f), vec3(0.5f,  0.5f,  0.5f)),
    triangleNormal(vec3(0.5f,  0.5f,  0.5f), vec3(-0.5f,  0.5f,  0.5f), vec3(-0.5f, -0.5f,  0.5f)),
    triangleNormal(vec3(0.5f,  0.5f,  0.5f), vec3(-0.5f,  0.5f,  0.5f), vec3(-0.5f, -0.5f,  0.5f)),
    triangleNormal(vec3(0.5f,  0.5f,  0.5f), vec3(-0.5f,  0.5f,  0.5f), vec3(-0.5f, -0.5f,  0.5f)),
    
    triangleNormal(vec3(-0.5f,  0.5f,  0.5f), vec3(-0.5f,  0.5f, -0.5f), vec3(-0.5f, -0.5f, -0.5f)),
    triangleNormal(vec3(-0.5f,  0.5f,  0.5f), vec3(-0.5f,  0.5f, -0.5f), vec3(-0.5f, -0.5f, -0.5f)),
    triangleNormal(vec3(-0.5f,  0.5f,  0.5f), vec3(-0.5f,  0.5f, -0.5f), vec3(-0.5f, -0.5f, -0.5f)),
    triangleNormal(vec3(-0.5f, -0.5f, -0.5f), vec3(-0.5f, -0.5f,  0.5f), vec3(-0.5f,  0.5f,  0.5f)),
    triangleNormal(vec3(-0.5f, -0.5f, -0.5f), vec3(-0.5f, -0.5f,  0.5f), vec3(-0.5f,  0.5f,  0.5f)),
    triangleNormal(vec3(-0.5f, -0.5f, -0.5f), vec3(-0.5f, -0.5f,  0.5f), vec3(-0.5f,  0.5f,  0.5f)),
    
    triangleNormal(vec3(0.5f,  0.5f,  0.5f), vec3(0.5f,  0.5f, -0.5f), vec3(0.5f, -0.5f, -0.5f)),
    triangleNormal(vec3(0.5f,  0.5f,  0.5f), vec3(0.5f,  0.5f, -0.5f), vec3(0.5f, -0.5f, -0.5f)),
    triangleNormal(vec3(0.5f,  0.5f,  0.5f), vec3(0.5f,  0.5f, -0.5f), vec3(0.5f, -0.5f, -0.5f)),
    triangleNormal(vec3(0.5f, -0.5f, -0.5f), vec3(0.5f, -0.5f,  0.5f), vec3(0.5f,  0.5f,  0.5f)),
    triangleNormal(vec3(0.5f, -0.5f, -0.5f), vec3(0.5f, -0.5f,  0.5f), vec3(0.5f,  0.5f,  0.5f)),
    triangleNormal(vec3(0.5f, -0.5f, -0.5f), vec3(0.5f, -0.5f,  0.5f), vec3(0.5f,  0.5f,  0.5f)),
    
    triangleNormal(vec3(-0.5f, -0.5f, -0.5f), vec3(0.5f, -0.5f, -0.5f), vec3(0.5f, -0.5f,  0.5f)),
    triangleNormal(vec3(-0.5f, -0.5f, -0.5f), vec3(0.5f, -0.5f, -0.5f), vec3(0.5f, -0.5f,  0.5f)),
    triangleNormal(vec3(-0.5f, -0.5f, -0.5f), vec3(0.5f, -0.5f, -0.5f), vec3(0.5f, -0.5f,  0.5f)),
    triangleNormal(vec3(0.5f, -0.5f,  0.5f), vec3(-0.5f, -0.5f,  0.5f), vec3(-0.5f, -0.5f, -0.5f)),
    triangleNormal(vec3(0.5f, -0.5f,  0.5f), vec3(-0.5f, -0.5f,  0.5f), vec3(-0.5f, -0.5f, -0.5f)),
    triangleNormal(vec3(0.5f, -0.5f,  0.5f), vec3(-0.5f, -0.5f,  0.5f), vec3(-0.5f, -0.5f, -0.5f)),
    
    triangleNormal(vec3(-0.5f,  0.5f, -0.5f), vec3(0.5f,  0.5f, -0.5f), vec3(0.5f,  0.5f,  0.5f)),
    triangleNormal(vec3(-0.5f,  0.5f, -0.5f), vec3(0.5f,  0.5f, -0.5f), vec3(0.5f,  0.5f,  0.5f)),
    triangleNormal(vec3(-0.5f,  0.5f, -0.5f), vec3(0.5f,  0.5f, -0.5f), vec3(0.5f,  0.5f,  0.5f)),
    triangleNormal(vec3(0.5f,  0.5f,  0.5f), vec3(-0.5f,  0.5f,  0.5f), vec3(-0.5f,  0.5f, -0.5f)),
    triangleNormal(vec3(0.5f,  0.5f,  0.5f), vec3(-0.5f,  0.5f,  0.5f), vec3(-0.5f,  0.5f, -0.5f)),
    triangleNormal(vec3(0.5f,  0.5f,  0.5f), vec3(-0.5f,  0.5f,  0.5f), vec3(-0.5f,  0.5f, -0.5f))
};

vec2 cubeTexCoords[] = {
    vec2(0.0f,  0.0f),
    vec2(1.0f,  0.0f),
    vec2(1.0f,  1.0f),
    vec2(1.0f,  1.0f),
    vec2(0.0f,  1.0f),
    vec2(0.0f,  0.0f),

    vec2(0.0f,  0.0f),
    vec2(1.0f,  0.0f),
    vec2(1.0f,  1.0f),
    vec2(1.0f,  1.0f),
    vec2(0.0f,  1.0f),
    vec2(0.0f,  0.0f),
    
    vec2(1.0f,  0.0f),
    vec2(1.0f,  1.0f),
    vec2(0.0f,  1.0f),
    vec2(0.0f,  1.0f),
    vec2(0.0f,  0.0f),
    vec2(1.0f,  0.0f),
 
    vec2(1.0f,  0.0f),
    vec2(1.0f,  1.0f),
    vec2(0.0f,  1.0f),
    vec2(0.0f,  1.0f),
    vec2(0.0f,  0.0f),
    vec2(1.0f,  0.0f),
   
    vec2(0.0f,  1.0f),
    vec2(1.0f,  1.0f),
    vec2(1.0f,  0.0f),
    vec2(1.0f,  0.0f),
    vec2(0.0f,  0.0f),
    vec2(0.0f,  1.0f),
   
    vec2(0.0f,  1.0f),
    vec2(1.0f,  1.0f),
    vec2(1.0f,  0.0f),
    vec2(1.0f,  0.0f),
    vec2(0.0f,  0.0f),
    vec2(0.0f,  1.0f)
};

Cube::Cube(Shader *shader, const RenderData *data):
shader(shader), vertex(cubeVertices, sizeof(cubeVertices), 0), texCoord(cubeTexCoords, sizeof(cubeTexCoords), 1), normals(cubeNormals, sizeof(cubeNormals), 2), translate(1), rotate(1), scale(1), model(1), position(vec3(0.0f)), size(vec3(1.0f)), data(data), tex(""), map(NULL) {
    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);
    
    vertex.activate();
    texCoord.activate();
    normals.activate();
    
    glBindVertexArray(0);
}

Cube::~Cube() {
    glDeleteVertexArrays(1, &this->VAO);
}

void Cube::render() {
    if(tex.getData()) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex.getData());
        shader->sendInt(0, tex.getTextureName());
    }
    else if(map.getData()) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, tex.getData());
        shader->sendInt(0, tex.getTextureName());
    }
    
    
    glBindVertexArray(VAO);
    shader->sendMat4(*data->projection, "projection");
    shader->sendMat4(data->viewMat, "view");
    shader->sendMat4(model, "model");
    
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void Cube::setTexture(Texture texture) {
    this->tex = texture;
}

void Cube::setCubemap(Cubemap texture) {
    this->map = texture;
}


void Cube::setPosition(vec3 position) {
    translate = glm::translate(mat4(1), position);
    model = translate * rotate * scale;
    this->position = position;
}

void Cube::setRotation(quat rotation) {
    rotate = toMat4(rotation);
    model = translate * rotate * scale;
    this->rotation = rotation;
}

void Cube::addRotation(quat rotation) {
    this->rotation = rotation * this->rotation;
    setRotation(this->rotation);
}

void Cube::setSize(vec3 size) {
    scale = glm::scale(mat4(1), size);
    model = translate * rotate * scale;
    this->size = size;
}

vec3 Cube::getPosition() {
    return position;
}

quat Cube::getRotation() {
    return rotation;
}

vec3 Cube::getSize() {
    return size;
}

Shader* Cube::getShaderPointer() {
    return shader;
}
