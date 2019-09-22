//
//  rectangle.cpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 17.03.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#include "rectangle.hpp"

static glm::vec3 rectangleVertices[] = {
    glm::vec3(-1.0f, 0.0f, -1.0f),
    glm::vec3(1.0f, 0.0f, -1.0f),
    glm::vec3(-1.0f, 0.0f, 1.0f),
    glm::vec3(1.0f, 0.0f, -1.0f),
    glm::vec3(-1.0f, 0.0f, 1.0f),
    glm::vec3(1.0f, 0.0f, 1.0f)
};

static glm::vec2 rectangleTextures[] = {
    glm::vec2(0.0f, 0.0f),
    glm::vec2(1.0f, 0.0f),
    glm::vec2(0.0f, 1.0f),
    glm::vec2(1.0f, 0.0f),
    glm::vec2(0.0f, 1.0f),
    glm::vec2(1.0f, 1.0f)
};

Rectangle::Rectangle(Shader *shader, const RenderData *data, bool *wireframe):
shader(shader), vertex(rectangleVertices, sizeof(rectangleVertices), 0), texCoord(rectangleTextures, sizeof(rectangleTextures), 1), model(1), translate(1), rotate(1), scale(1), position(glm::vec3(0.0f)), size(glm::vec3(1.0f)), tex(nullptr), data(data), wireframe(wireframe) {
    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);
    
    vertex.activate();
    texCoord.activate();
    
    glBindVertexArray(0);
}

Rectangle::~Rectangle() {
    glDeleteVertexArrays(1, &this->VAO);
}

void Rectangle::render() {
    vertex.activate();
    texCoord.activate();
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex->getTextureID());
    shader->sendInt(0, tex->getTextureName());
    
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
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void Rectangle::setTexture(Texture *texture) {
    tex = texture;
}

void Rectangle::setPosition(glm::vec3 position) {
    translate = glm::translate(glm::mat4(1), position);
    model = translate * rotate * scale;
    this->position = position;
}

void Rectangle::setRotation(glm::vec4 rotation) {
    rotate = glm::rotate(glm::mat4(1), rotation.w, rotation.xyz());
    model = translate * rotate * scale;
    this->rotation = rotation;
}

void Rectangle::setSize(glm::vec3 size) {
    scale = glm::scale(glm::mat4(1), size);
    model = translate * rotate * scale;
    this->size = size;
}

void Rectangle::setModelMat(glm::mat4 modelMat) {
    this->model = modelMat;
}


glm::vec3 Rectangle::getPosition() {
    return position;
}

glm::vec3 Rectangle::getRealPosition() {
    return position;
}

glm::vec4 Rectangle::getRotation() {
    return rotation;
}

glm::vec3 Rectangle::getSize() {
    return size;
}

Shader* Rectangle::getShaderPointer() {
    return shader;
}
