//
//  cube.cpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 16.04.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#include "cube.hpp"









































static glm::vec3 cubeVertices[] = {
    glm::vec3(-0.5f, -0.5f, -0.5f),
    glm::vec3( 0.5f, -0.5f, -0.5f),
    glm::vec3( 0.5f,  0.5f, -0.5f),
    glm::vec3( 0.5f,  0.5f, -0.5f),
    glm::vec3(-0.5f,  0.5f, -0.5f),
    glm::vec3(-0.5f, -0.5f, -0.5f),
    
    glm::vec3(-0.5f, -0.5f,  0.5f),
    glm::vec3( 0.5f, -0.5f,  0.5f),
    glm::vec3( 0.5f,  0.5f,  0.5f),
    glm::vec3( 0.5f,  0.5f,  0.5f),
    glm::vec3(-0.5f,  0.5f,  0.5f),
    glm::vec3(-0.5f, -0.5f,  0.5f),
    
    glm::vec3(-0.5f,  0.5f,  0.5f),
    glm::vec3(-0.5f,  0.5f, -0.5f),
    glm::vec3(-0.5f, -0.5f, -0.5f),
    glm::vec3(-0.5f, -0.5f, -0.5f),
    glm::vec3(-0.5f, -0.5f,  0.5f),
    glm::vec3(-0.5f,  0.5f,  0.5f),
    
    glm::vec3( 0.5f,  0.5f,  0.5f),
    glm::vec3( 0.5f,  0.5f, -0.5f),
    glm::vec3( 0.5f, -0.5f, -0.5f),
    glm::vec3( 0.5f, -0.5f, -0.5f),
    glm::vec3( 0.5f, -0.5f,  0.5f),
    glm::vec3( 0.5f,  0.5f,  0.5f),
    
    glm::vec3(-0.5f, -0.5f, -0.5f),
    glm::vec3( 0.5f, -0.5f, -0.5f),
    glm::vec3( 0.5f, -0.5f,  0.5f),
    glm::vec3( 0.5f, -0.5f,  0.5f),
    glm::vec3(-0.5f, -0.5f,  0.5f),
    glm::vec3(-0.5f, -0.5f, -0.5f),
    
    glm::vec3(-0.5f,  0.5f, -0.5f),
    glm::vec3( 0.5f,  0.5f, -0.5f),
    glm::vec3( 0.5f,  0.5f,  0.5f),
    glm::vec3( 0.5f,  0.5f,  0.5f),
    glm::vec3(-0.5f,  0.5f,  0.5f),
    glm::vec3(-0.5f,  0.5f, -0.5f)
};

static glm::vec3 cubeNormals[] = {
    glm::vec3( 0.0f,  0.0f, -1.0f),
    glm::vec3( 0.0f,  0.0f, -1.0f),
    glm::vec3( 0.0f,  0.0f, -1.0f),
    glm::vec3( 0.0f,  0.0f, -1.0f),
    glm::vec3( 0.0f,  0.0f, -1.0f),
    glm::vec3( 0.0f,  0.0f, -1.0f),
    glm::vec3( 0.0f,  0.0f,  1.0f),
    glm::vec3( 0.0f,  0.0f,  1.0f),
    glm::vec3( 0.0f,  0.0f,  1.0f),
    glm::vec3( 0.0f,  0.0f,  1.0f),
    glm::vec3( 0.0f,  0.0f,  1.0f),
    glm::vec3( 0.0f,  0.0f,  1.0f),
    glm::vec3(-1.0f,  0.0f,  0.0f),
    glm::vec3(-1.0f,  0.0f,  0.0f),
    glm::vec3(-1.0f,  0.0f,  0.0f),
    glm::vec3(-1.0f,  0.0f,  0.0f),
    glm::vec3(-1.0f,  0.0f,  0.0f),
    glm::vec3(-1.0f,  0.0f,  0.0f),
    glm::vec3( 1.0f,  0.0f,  0.0f),
    glm::vec3( 1.0f,  0.0f,  0.0f),
    glm::vec3( 1.0f,  0.0f,  0.0f),
    glm::vec3( 1.0f,  0.0f,  0.0f),
    glm::vec3( 1.0f,  0.0f,  0.0f),
    glm::vec3( 1.0f,  0.0f,  0.0f),
    glm::vec3( 0.0f, -1.0f,  0.0f),
    glm::vec3( 0.0f, -1.0f,  0.0f),
    glm::vec3( 0.0f, -1.0f,  0.0f),
    glm::vec3( 0.0f, -1.0f,  0.0f),
    glm::vec3( 0.0f, -1.0f,  0.0f),
    glm::vec3( 0.0f, -1.0f,  0.0f),
    glm::vec3( 0.0f,  1.0f,  0.0f),
    glm::vec3( 0.0f,  1.0f,  0.0f),
    glm::vec3( 0.0f,  1.0f,  0.0f),
    glm::vec3( 0.0f,  1.0f,  0.0f),
    glm::vec3( 0.0f,  1.0f,  0.0f),
    glm::vec3( 0.0f,  1.0f,  0.0f)
};

static glm::vec2 cubeTexCoords[] = {
    glm::vec2(0.0f,  0.0f),
    glm::vec2(1.0f,  0.0f),
    glm::vec2(1.0f,  1.0f),
    glm::vec2(1.0f,  1.0f),
    glm::vec2(0.0f,  1.0f),
    glm::vec2(0.0f,  0.0f),

    glm::vec2(0.0f,  0.0f),
    glm::vec2(1.0f,  0.0f),
    glm::vec2(1.0f,  1.0f),
    glm::vec2(1.0f,  1.0f),
    glm::vec2(0.0f,  1.0f),
    glm::vec2(0.0f,  0.0f),
    
    glm::vec2(1.0f,  0.0f),
    glm::vec2(1.0f,  1.0f),
    glm::vec2(0.0f,  1.0f),
    glm::vec2(0.0f,  1.0f),
    glm::vec2(0.0f,  0.0f),
    glm::vec2(1.0f,  0.0f),
 
    glm::vec2(1.0f,  0.0f),
    glm::vec2(1.0f,  1.0f),
    glm::vec2(0.0f,  1.0f),
    glm::vec2(0.0f,  1.0f),
    glm::vec2(0.0f,  0.0f),
    glm::vec2(1.0f,  0.0f),
   
    glm::vec2(0.0f,  1.0f),
    glm::vec2(1.0f,  1.0f),
    glm::vec2(1.0f,  0.0f),
    glm::vec2(1.0f,  0.0f),
    glm::vec2(0.0f,  0.0f),
    glm::vec2(0.0f,  1.0f),
   
    glm::vec2(0.0f,  1.0f),
    glm::vec2(1.0f,  1.0f),
    glm::vec2(1.0f,  0.0f),
    glm::vec2(1.0f,  0.0f),
    glm::vec2(0.0f,  0.0f),
    glm::vec2(0.0f,  1.0f)
};

Cube::Cube(Shader *shader, const RenderData *data, bool *wireframe):
shader(shader), vertex(cubeVertices, sizeof(cubeVertices), 0), texCoord(cubeTexCoords, sizeof(cubeTexCoords), 1), normals(cubeNormals, sizeof(cubeNormals), 2), translate(1), rotate(1), scale(1), model(1), position(glm::vec3(0.0f)), size(glm::vec3(1.0f)), data(data), tex(nullptr), map(nullptr), wireframe(wireframe) {
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
    vertex.activate();
    texCoord.activate();
    normals.activate();
    
    if(this->tex != nullptr) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex->getTextureID());
    }

    if(this->map != nullptr) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, map->getData());
    }
    
    
    glBindVertexArray(VAO);
    if(wireframe != nullptr) {
        shader->sendInt(*wireframe, "wireframe");
    }
    else {
        shader->sendInt(0, "wireframe");
    }
    
    shader->sendMat4(*data->projection, "projection");
    shader->sendMat4(data->viewMat, "view");
    shader->sendMat4(model, "model");
    
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void Cube::setTexture(Texture *texture) {
    this->tex = texture;
}

void Cube::setCubemap(Cubemap *texture) {
    this->map = texture;
}


void Cube::setPosition(glm::vec3 position) {
    translate = glm::translate(glm::mat4(1), position);
    model = translate * rotate * scale;
    this->position = position;
}

void Cube::setRotation(glm::vec4 rotation) {
    rotate = glm::rotate(glm::mat4(1), rotation.w, rotation.xyz());
    model = translate * rotate * scale;
    this->rotation = rotation;
}

void Cube::setSize(glm::vec3 size) {
    scale = glm::scale(glm::mat4(1), size);
    model = translate * rotate * scale;
    this->size = size;
}

void Cube::setModelMat(glm::mat4 modelMat) {
    this->model = modelMat;
}


glm::vec3 Cube::getPosition() {
    return position;
}

glm::vec3 Cube::getRealPosition() {
    return position;
}

glm::vec4 Cube::getRotation() {
    return rotation;
}

glm::vec3 Cube::getSize() {
    return size;
}

Shader* Cube::getShaderPointer() {
    return shader;
}
