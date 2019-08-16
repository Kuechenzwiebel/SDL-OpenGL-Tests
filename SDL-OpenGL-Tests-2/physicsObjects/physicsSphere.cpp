//
//  physicsSphere.cpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 05.08.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#include "physicsSphere.hpp"

PhysicsSphere::PhysicsSphere(float radius, glm::vec3 position):
radius(radius), position(position) {
    type = SPHERE_t;
}

float PhysicsSphere::getRadius() {
    return radius;
}

glm::vec3 PhysicsSphere::getPosition() {
    return position;
}


CollisionInfo sphereSphereCollision(PhysicsSphere *sp1, PhysicsSphere *sp2) {
    CollisionInfo info;
    
    float minDistance = sp1->getRadius() + sp2->getRadius();
    glm::vec3 direction = sp2->getPosition() - sp1->getPosition();
    float centerDistance = glm::length(direction);
    direction = glm::normalize(direction);
    float distance = centerDistance - minDistance;
    
    if(centerDistance < minDistance) {
        info.collision = true;
        info.collisionDepth = -centerDistance - minDistance * minDistance;
        info.collisionPosition = direction * distance;
    }
    else {
        info.collision = false;
    }
    
    return info;
}

CollisionInfo spherePointCollision(PhysicsSphere *sp1, glm::vec3 point) {
    CollisionInfo info;
    
    float minDistance = sp1->getRadius();
    glm::vec3 direction = point - sp1->getPosition();
    float centerDistance = glm::length(direction);
    direction = glm::normalize(direction);
    float distance = centerDistance - minDistance;
    
    if(centerDistance < minDistance) {
        info.collision = true;
        info.collisionDepth = -centerDistance - minDistance * minDistance;
        info.collisionPosition = direction * distance;
    }
    else {
        info.collision = false;
    }
    
    return info;
}
