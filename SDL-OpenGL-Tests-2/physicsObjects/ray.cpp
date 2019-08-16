//
//  ray.cpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 16.08.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#include "ray.hpp"

Ray::Ray(glm::vec3 rayStartPosition, glm::vec3 rayDirection, float stepSize):
rayPosition(rayStartPosition), rayStartPosition(rayStartPosition), rayDirection(rayDirection), stepSize(stepSize) {
    stepDirection = glm::normalize(rayDirection - rayStartPosition) * stepSize;
}

void Ray::step() {
    rayPosition += stepDirection;
}

void Ray::reset() {
    rayPosition = rayStartPosition;
}

void Ray::setRayStartPosition(glm::vec3 position) {
    this->rayStartPosition = position;
    stepDirection = glm::normalize(rayDirection - rayStartPosition) * stepSize;
}

void Ray::setRayDirection(glm::vec3 direction) {
    this->rayDirection = direction;
    stepDirection = glm::normalize(rayDirection - rayStartPosition) * stepSize;
}

glm::vec3 Ray::getRayPosition() {
    return rayPosition;
}
