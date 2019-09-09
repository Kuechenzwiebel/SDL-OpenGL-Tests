//
//  triangle.cpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 08.03.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#include "triangle.hpp"

static glm::vec3 triangleVertices[] = {
    glm::vec3(-0.5f, -0.5f, 0.0f),
    glm::vec3(0.5f, -0.5f, 0.0f),
    glm::vec3(0.0f,  0.5f, 0.0f)
};

static glm::vec4 triangleColors[] = {
    glm::vec4(1.0f, 0.0f, 0.0f, 0.0f),
    glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
    glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)
};

Triangle::Triangle(Shader *shader, const RenderData *data, bool *wireframe):
shader(shader), vertex(triangleVertices, sizeof(triangleVertices), 0), texCoord(triangleColors, sizeof(triangleColors), 1), model(1), position(glm::vec3(0.0f)),
angle(0.0f), rotationAxis(glm::vec3(1.0f)), size(glm::vec3(1.0f)), data(data), wireframe(wireframe) {
    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);
    
    vertex.activate();
    texCoord.activate();
    
    glBindVertexArray(0);
}

Triangle::~Triangle() {
    glDeleteVertexArrays(1, &this->VAO);
}

void Triangle::render() {
    vertex.activate();
    texCoord.activate();
    
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
    
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

void Triangle::calculateModelMat() {
    model = glm::mat4(1);
    model = translate(model, position);
    model = rotate(model, angle, rotationAxis);
    model = scale(model, size);
}

void Triangle::setPosition(glm::vec3 position) {
    this->position = position;
}

void Triangle::setAngle(float angle) {
    this->angle = angle;
}

void Triangle::setRotationAxis(glm::vec3 rotationAxis) {
    this->rotationAxis = rotationAxis;
}

void Triangle::setSize(glm::vec3 size) {
    this->size = size;
}

glm::vec3 Triangle::getPosition() {
    return position;
}

float Triangle::getAngle() {
    return angle;
}

glm::vec3 Triangle::getRotationAxis() {
    return rotationAxis;
}

glm::vec3 Triangle::getSize() {
    return size;
}

Shader* Triangle::getShaderPointer() {
    return shader;
}
