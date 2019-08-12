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
    float centerDistance = length((sp2->getPosition() - sp1->getPosition()));
    
    if(centerDistance < minDistance) {
        info.collision = true;
        info.collisionDepth = -centerDistance - minDistance;
        info.collisionPosition = sp2->getPosition() - 0.41f;
    }
    else {
        info.collision = false;
    }
    
    return info;
}

CollisionInfo spherePointCollision(PhysicsSphere *sp1, glm::vec3 point) {
    CollisionInfo info;
    
    float centerDistance = length((point - sp1->getPosition()));
    
    if(centerDistance < sp1->getRadius()) {
        info.collision = true;
//        info.collisionDepth = centerDistance - minDistance;
//        info.collisionPosition = sp2->getPosition() - 0.41f;
    }
    else {
        info.collision = false;
    }
    
    return info;
}
