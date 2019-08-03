//
//  pointLightSource.cpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 24.07.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#include "pointLightSource.hpp"

PointLightSource::PointLightSource(Object *renderObject, Shader *shaders, int shaderAmount):
renderObject(renderObject), shaders(shaders), shaderAmount(shaderAmount) {
    
}

void PointLightSource::render() {
    if(renderObject != nullptr) {
        renderObject->getShaderPointer()->use();
        renderObject->render();
    }
}

void PointLightSource::setPosition(vec3 position) {
    this->position = position;
    
    if(renderObject != nullptr) {
        renderObject->setPosition(position - renderObject->getSize() * vec3(0.5f));
    }
}

vec3 PointLightSource::getPosition() {
    return position;
}

