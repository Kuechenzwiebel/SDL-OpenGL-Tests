//
//  sphere.cpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 22.06.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#include "sphere.hpp"

glm::vec3 Sphere::sphereVertices[sphereArraySize];
glm::vec2 Sphere::sphereUVs[sphereArraySize];
glm::vec3 Sphere::sphereNormals[sphereArraySize];

static glm::vec3 sphereSurface(float angleXZ, float angleXY) {
    glm::vec3 pos;
    pos.x = sin(glm::radians(angleXY)) * cos(glm::radians(angleXZ));
    pos.z = sin(glm::radians(angleXY)) * sin(glm::radians(angleXZ));
    pos.y = cos(glm::radians(angleXY));
    return pos;
}

Sphere::Sphere(Shader *shader, const RenderData *data):
shader(shader), vertex(), colorBuffer(), translate(1), rotate(1), scale(1), model(1), position(glm::vec3(0.0f)), radius(1.0f), rotation(), data(data), tex(nullptr) {
    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);
    
    int i = 0;
    
    for(float angleXY = 0.0f; angleXY < 180.0f; angleXY += sphereResolution) {
        for(float angleXZ = 0.0f; angleXZ < 360.0f; angleXZ += sphereResolution) {
            sphereVertices[i + 0] = sphereSurface(angleXZ + (0.0f * sphereResolution), angleXY + (0.0f * sphereResolution));
            sphereVertices[i + 1] = sphereSurface(angleXZ + (1.0f * sphereResolution), angleXY + (0.0f * sphereResolution));
            sphereVertices[i + 2] = sphereSurface(angleXZ + (0.0f * sphereResolution), angleXY + (1.0f * sphereResolution));
            
            sphereVertices[i + 3] = sphereSurface(angleXZ + (1.0f * sphereResolution), angleXY + (0.0f * sphereResolution));
            sphereVertices[i + 4] = sphereSurface(angleXZ + (0.0f * sphereResolution), angleXY + (1.0f * sphereResolution));
            sphereVertices[i + 5] = sphereSurface(angleXZ + (1.0f * sphereResolution), angleXY + (1.0f * sphereResolution));
            
            
            sphereNormals[i + 0] = glm::triangleNormal(sphereVertices[i + 0], sphereVertices[i + 1], sphereVertices[i + 2]) * -1.0f;
            sphereNormals[i + 1] = glm::triangleNormal(sphereVertices[i + 0], sphereVertices[i + 1], sphereVertices[i + 2]) * -1.0f;
            sphereNormals[i + 2] = glm::triangleNormal(sphereVertices[i + 0], sphereVertices[i + 1], sphereVertices[i + 2]) * -1.0f;
           
            sphereNormals[i + 3] = glm::triangleNormal(sphereVertices[i + 5], sphereVertices[i + 4], sphereVertices[i + 3]) * -1.0f;
            sphereNormals[i + 4] = glm::triangleNormal(sphereVertices[i + 5], sphereVertices[i + 4], sphereVertices[i + 3]) * -1.0f;
            sphereNormals[i + 5] = glm::triangleNormal(sphereVertices[i + 5], sphereVertices[i + 4], sphereVertices[i + 3]) * -1.0f;
            
            
            
            sphereUVs[i + 0] = glm::vec2(1.0f - (angleXZ + (0.0f * sphereResolution)) / 360.0f, 1.0f - (angleXY + (0.0f * sphereResolution)) / 180.0f);
            sphereUVs[i + 1] = glm::vec2(1.0f - (angleXZ + (1.0f * sphereResolution)) / 360.0f, 1.0f - (angleXY + (0.0f * sphereResolution)) / 180.0f);
            sphereUVs[i + 2] = glm::vec2(1.0f - (angleXZ + (0.0f * sphereResolution)) / 360.0f, 1.0f - (angleXY + (1.0f * sphereResolution)) / 180.0f);
            
            sphereUVs[i + 3] = glm::vec2(1.0f - (angleXZ + (1.0f * sphereResolution)) / 360.0f, 1.0f - (angleXY + (0.0f * sphereResolution)) / 180.0f);
            sphereUVs[i + 4] = glm::vec2(1.0f - (angleXZ + (0.0f * sphereResolution)) / 360.0f, 1.0f - (angleXY + (1.0f * sphereResolution)) / 180.0f);
            sphereUVs[i + 5] = glm::vec2(1.0f - (angleXZ + (1.0f * sphereResolution)) / 360.0f, 1.0f - (angleXY + (1.0f * sphereResolution)) / 180.0f);
            
            
            i += 6;
        }
    }
    
    vertex.setData(sphereVertices, sphereArraySize * sizeof(glm::vec3), 0);
    colorBuffer.setData(sphereUVs, sphereArraySize * sizeof(glm::vec2), 1);
    normal.setData(sphereNormals, sphereArraySize * sizeof(glm::vec3), 2);
    
    vertex.activate();
    colorBuffer.activate();
    normal.activate();
    
    glBindVertexArray(0);
}

Sphere::~Sphere() {
    glDeleteVertexArrays(1, &this->VAO);
}

void Sphere::setTexture(Texture *texture) {
    this->tex = texture;
}

void Sphere::render() {
    vertex.activate();
    colorBuffer.activate();
    normal.activate();
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex->getTextureID());
    shader->sendInt(0, tex->getTextureName());
    
    glBindVertexArray(VAO);
    shader->sendMat4(*data->projection, "projection");
    shader->sendMat4(data->viewMat, "view");
    shader->sendMat4(model, "model");
    
    glDrawArrays(GL_TRIANGLES, 0, sphereArraySize);
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
