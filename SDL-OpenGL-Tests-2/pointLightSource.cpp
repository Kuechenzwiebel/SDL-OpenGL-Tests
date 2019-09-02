//
//  pointLightSource.cpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 24.07.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#include "pointLightSource.hpp"

PointLightSource::PointLightSource(Object *renderObject, std::vector<Shader*> *shaders):
renderObject(renderObject), shaders(shaders) {
    
}

void PointLightSource::render() {
    if(renderObject != nullptr) {
        renderObject->getShaderPointer()->use();
        renderObject->render();
    }
}

void PointLightSource::setPosition(glm::vec3 position) {
    this->position = position;
    
    if(renderObject != nullptr) {
        renderObject->setPosition(position - renderObject->getSize() * glm::vec3(0.5f));
    }
    
    for(int i = 0; i < shaders->size(); i++) {
        
    }
}

glm::vec3 PointLightSource::getPosition() {
    return position;
}

