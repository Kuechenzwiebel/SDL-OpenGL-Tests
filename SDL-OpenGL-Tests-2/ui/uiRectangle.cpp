//
//  rectangle.cpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 30.03.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#include "uiRectangle.hpp"

static glm::vec3 uiRectangleVertices[] = {
    glm::vec3(-1.0f, -1.0f, 0.0f),
    glm::vec3(1.0f,  -1.0f, 0.0f),
    glm::vec3(1.0f,   1.0f, 0.0f),
    glm::vec3(-1.0f, -1.0f, 0.0f),
    glm::vec3(-1.0f,  1.0f, 0.0f),
    glm::vec3(1.0f,   1.0f, 0.0f)
};

static glm::vec2 uiRectangleTextures[] = {
    glm::vec2(0.0f, 0.0f),
    glm::vec2(1.0f, 0.0f),
    glm::vec2(1.0f, 1.0f),
    glm::vec2(0.0f, 0.0f),
    glm::vec2(0.0f, 1.0f),
    glm::vec2(1.0f, 1.0f)
};

UIRectangle::UIRectangle(Shader *shader, const RenderData *data):
shader(shader), vertex(uiRectangleVertices, sizeof(uiRectangleVertices), 0), texCoord(uiRectangleTextures, 6 * sizeof(glm::vec2), 1), tex(""), data(data), position(0.0f), size(1.0f), model(1), translate(1), scale(1), rotate(1), texMultiplier(glm::vec2(1.0f)),  noXTexOffset(true), noYTexOffset(true) {
    
    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);
    
    vertex.activate();
    texCoord.activate();
    
    glBindVertexArray(0);
}

UIRectangle::UIRectangle(Shader *shader, const RenderData *data, const glm::vec2 *customUVs):
shader(shader), vertex(uiRectangleVertices, sizeof(uiRectangleVertices), 0), texCoord(customUVs, 6 * sizeof(glm::vec2), 1), tex(""), data(data), position(0.0f), size(1.0f), model(1), translate(1), scale(1), rotate(1), texMultiplier(glm::vec2(1.0f)), noXTexOffset(true), noYTexOffset(true) {
    
    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);
    
    vertex.activate();
    texCoord.activate();
    
    glBindVertexArray(0);
}

UIRectangle::UIRectangle():
shader(nullptr), tex(""), position(0.0f), size(1.0f), model(1), translate(1), scale(1), rotate(1), texMultiplier(glm::vec2(1.0f)), noXTexOffset(true), noYTexOffset(true) {
    
}

void UIRectangle::set(Shader *shader, const RenderData *data) {
    this->shader = shader;
    this->vertex.setData(uiRectangleVertices, sizeof(uiRectangleVertices), 0);
    this->texCoord.setData(uiRectangleTextures, 6 * sizeof(glm::vec2), 1);
    this->data = data;
    
    
    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);
    
    vertex.activate();
    texCoord.activate();
    
    glBindVertexArray(0);
}

void UIRectangle::set(Shader *shader, const RenderData *data, const glm::vec2 *customUVs) {
    this->shader = shader;
    this->vertex.setData(uiRectangleVertices, sizeof(uiRectangleVertices), 0);
    this->texCoord.setData(customUVs, 6 * sizeof(glm::vec2), 1);
    this->data = data;
    
    
    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);
    
    vertex.activate();
    texCoord.activate();
    
    glBindVertexArray(0);
}

UIRectangle::~UIRectangle() {
    glDeleteVertexArrays(1, &this->VAO);
}

void UIRectangle::render() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex.getData());
    shader->sendInt(0, tex.getTextureName());
    
    glBindVertexArray(VAO);
    
    if(noXTexOffset) {
        texOffset.x = 0.0f;
    }
    else {
        texOffset.x = *xTexOffset;
    }
    
    if(noYTexOffset) {
        texOffset.y = 0.0f;
    }
    else {
        texOffset.y = *yTexOffset;
    }
    
    shader->sendVec2(texOffset * texMultiplier, "texOffset");
    shader->sendMat4(*data->projection, "projection");
    shader->sendMat4(data->viewMat, "view");
    shader->sendMat4(model, "model");
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void UIRectangle::changeUVs(const glm::vec2 *UVs) {
    texCoord.changeData(UVs, 6 * sizeof(glm::vec2), 1);
    texCoord.activate();
}

void UIRectangle::setXTexMultiplier(float x) {
    texMultiplier.x = x;
}

void UIRectangle::setYTexMultiplier(float y) {
    texMultiplier.y = y;
}

void UIRectangle::setXTexOffset(float *x) {
    if(x == nullptr) {
        noXTexOffset = true;
    }
    else {
        xTexOffset = x;
        noXTexOffset = false;
    }
}

void UIRectangle::setYTexOffset(float *y) {
    if(y == nullptr) {
        noYTexOffset = true;
    }
    else {
        yTexOffset = y;
        noYTexOffset = false;
    }
}

void UIRectangle::setXTexOffset(float x) {
    rxTexOffset = x;
    xTexOffset = &rxTexOffset;
    noXTexOffset = false;
}

void UIRectangle::setYTexOffset(float y) {
    ryTexOffset = y;
    yTexOffset = &ryTexOffset;
    noYTexOffset = false;
}

void UIRectangle::setTexture(Texture texture) {
    this->tex = texture;
}

Texture UIRectangle::getTexture() {
    return tex;
}

void UIRectangle::setPixelPosition(glm::vec2 position) {
    this->position = pixelPosToUIRect(position, size);
    setPosition(this->position);
}

void UIRectangle::setPixelSize(glm::vec2 size) {
    this->size = pixelSizeToUIRect(size);
    setSize(this->size);
}

void UIRectangle::setPosition(glm::vec2 position) {
    translate = glm::translate(glm::mat4(1), glm::vec3(position, 0.0f));
    model = rotate * scale * translate;
    this->position = position;
}

void UIRectangle::setSize(glm::vec2 size) {
    scale = glm::scale(glm::mat4(1), glm::vec3(size, 1.0f));
    model = rotate * scale * translate;
    this->size = size;
}

void UIRectangle::setRotation(glm::quat rotation) {
    rotate = toMat4(rotation);
    model = rotate * scale * translate;
    this->rotation = rotation;
}

void UIRectangle::addRotation(glm::quat rotation) {
    this->rotation = rotation * this->rotation;
    setRotation(this->rotation);
}

glm::vec2 UIRectangle::getPosition() {
    return position;
}

glm::quat UIRectangle::getRotation() {
    return rotation;
}

glm::vec2 UIRectangle::getSize() {
    return size;
}

float UIRectangle::getXTexOffset() {
    return texOffset.x;
}

float UIRectangle::getYTexOffset() {
    return texOffset.y;
}

glm::mat4 UIRectangle::getModelMat() {
    return model;
}

Shader* UIRectangle::getShaderPointer() {
    return shader;
}
