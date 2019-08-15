//
//  sphere.cpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 22.06.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#include "sphere.hpp"

glm::vec3 sphereSurface(float angleXZ, float angleXY) {
    glm::vec3 pos;
    pos.x = sin(glm::radians(angleXY)) * cos(glm::radians(angleXZ));
    pos.z = sin(glm::radians(angleXY)) * sin(glm::radians(angleXZ));
    pos.y = cos(glm::radians(angleXY));
    return pos;
}

void swap(Sphere &first, Sphere &second) {
    using std::swap;
    
    swap(first.VAO, second.VAO);
    swap(first.sphereVertices, second.sphereVertices);
    swap(first.sphereUVs, second.sphereUVs);
    swap(first.tex, second.tex);
    swap(first.position, second.position);
    swap(first.rotation, second.rotation);
    swap(first.radius, second.radius);
    swap(first.translate, second.translate);
    swap(first.rotate, second.rotate);
    swap(first.scale, second.scale);
    swap(first.model, second.model);
    swap(first.vertex, second.vertex);
    swap(first.colorBuffer, second.colorBuffer);
    swap(first.resolution, second.resolution);
    swap(first.shader, second.shader);
    swap(first.data, second.data);
    
    first.vertex.activate();
    first.colorBuffer.activate();
    
    second.vertex.activate();
    second.colorBuffer.activate();
}

Sphere::Sphere(Shader *shader, const RenderData *data):
shader(shader), vertex(), colorBuffer(), translate(1), rotate(1), scale(1), model(1), position(glm::vec3(0.0f)), radius(1.0f), rotation(), data(data), tex("") {
    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);
    
    int i = 0;
    resolution = 10.0f;
    
    sphereVertices = new glm::vec3[int(round((360.0f / resolution) * (360.0f / resolution) * 6 * 0.5))];
    sphereUVs = new glm::vec2[int(round((360.0f / resolution) * (360.0f / resolution) * 6 * 0.5))];
  
    for(float angleXY = 0.0f; angleXY < 180.0f; angleXY += resolution) {
        for(float angleXZ = 0.0f; angleXZ < 360.0f; angleXZ += resolution) {
            sphereVertices[i + 0] = sphereSurface(angleXZ + (0.0f * resolution), angleXY + (0.0f * resolution));
            sphereVertices[i + 1] = sphereSurface(angleXZ + (1.0f * resolution), angleXY + (0.0f * resolution));
            sphereVertices[i + 2] = sphereSurface(angleXZ + (0.0f * resolution), angleXY + (1.0f * resolution));
            sphereVertices[i + 3] = sphereSurface(angleXZ + (1.0f * resolution), angleXY + (0.0f * resolution));
            sphereVertices[i + 4] = sphereSurface(angleXZ + (0.0f * resolution), angleXY + (1.0f * resolution));
            sphereVertices[i + 5] = sphereSurface(angleXZ + (1.0f * resolution), angleXY + (1.0f * resolution));
            
            sphereUVs[i + 0] = glm::vec2(1.0f - (angleXZ + (0.0f * resolution)) / 360.0f, 1.0f - (angleXY + (0.0f * resolution)) / 180.0f);
            sphereUVs[i + 1] = glm::vec2(1.0f - (angleXZ + (1.0f * resolution)) / 360.0f, 1.0f - (angleXY + (0.0f * resolution)) / 180.0f);
            sphereUVs[i + 2] = glm::vec2(1.0f - (angleXZ + (0.0f * resolution)) / 360.0f, 1.0f - (angleXY + (1.0f * resolution)) / 180.0f);
            sphereUVs[i + 3] = glm::vec2(1.0f - (angleXZ + (1.0f * resolution)) / 360.0f, 1.0f - (angleXY + (0.0f * resolution)) / 180.0f);
            sphereUVs[i + 4] = glm::vec2(1.0f - (angleXZ + (0.0f * resolution)) / 360.0f, 1.0f - (angleXY + (1.0f * resolution)) / 180.0f);
            sphereUVs[i + 5] = glm::vec2(1.0f - (angleXZ + (1.0f * resolution)) / 360.0f, 1.0f - (angleXY + (1.0f * resolution)) / 180.0f);
            
            i += 6;
        }
    }
    
    vertex.setData(sphereVertices, round(sizeof(glm::vec3) * (360.0f / resolution) * (360.0f / resolution) * 6 * 0.5), 0);
    colorBuffer.setData(sphereUVs, round(sizeof(glm::vec2) * (360.0f / resolution) * (360.0f / resolution) * 6 * 0.5), 1);
    
    vertex.activate();
    colorBuffer.activate();
    
    glBindVertexArray(0);
}

Sphere::Sphere(const Sphere& other):
tex("") {
    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);
    int arraySize = round((360.0f / other.resolution) * (360.0f / other.resolution) * 6 * 0.5);
    sphereVertices = new glm::vec3[arraySize];
    sphereUVs = new glm::vec2[arraySize];
    
    std::copy(other.sphereVertices, other.sphereVertices + arraySize, sphereVertices);
    std::copy(other.sphereUVs, other.sphereUVs + arraySize, sphereUVs);
    
    tex = other.tex;
    position = other.position;
    rotation = other.rotation;
    radius = other.radius;
    translate = other.translate;
    rotate = other.rotate;
    scale = other.scale;
    model = other.model;
    vertex = other.vertex;
    colorBuffer = other.colorBuffer;
    resolution = other.resolution;
    shader = other.shader;
    data = other.data;
    
    vertex.activate();
    colorBuffer.activate();
    
    glBindVertexArray(0);
}

Sphere& Sphere::operator=(Sphere other) {
    swap(*this, other);
    return *this;
}

Sphere::Sphere(Sphere &&other):
Sphere(nullptr, nullptr) {
    swap(*this, other);
}

Sphere::~Sphere() {
    glDeleteVertexArrays(1, &this->VAO);
    
    delete [] sphereVertices;
    delete [] sphereUVs;
}

void Sphere::setTexture(Texture texture) {
    this->tex = texture;
}

void Sphere::render() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex.getData());
    shader->sendInt(0, tex.getTextureName());
    
    glBindVertexArray(VAO);
    shader->sendMat4(*data->projection, "projection");
    shader->sendMat4(data->viewMat, "view");
    shader->sendMat4(model, "model");
    
    glDrawArrays(GL_TRIANGLES, 0, round((360.0f / resolution) * (360.0f / resolution) * 6 * 0.5));
    glBindVertexArray(0);
}

void Sphere::setPosition(glm::vec3 position) {
    translate = glm::translate(glm::mat4(1), position);
    model = translate * rotate * scale;
    this->position = position;
}

void Sphere::setRotation(glm::vec4 rotation) {
    rotate = glm::translate(glm::mat4(1), position);
    model = translate * rotate * scale;
    this->rotation = rotation;
}

void Sphere::setRadius(float radius) {
    scale = glm::scale(glm::mat4(1), glm::vec3(radius));
    model = translate * rotate * scale;
    this->radius = radius;
}

void Sphere::setModelMat(glm::mat4 model) {
    this->model = model;
}

glm::vec3 Sphere::getPosition() {
    return position;
}

glm::vec4 Sphere::getRotation() {
    return rotation;
}

float Sphere::getRadius() {
    return radius;
}

Shader* Sphere::getShaderPointer() {
    return shader;
}
