//
//  physicsSphere.cpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 05.08.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#include "physicsSphere.hpp"

PhysicsSphere::PhysicsSphere(float radius, vec3 position):
radius(radius), position(position) {
    
}

float PhysicsSphere::getRadius() {
    return radius;
}

vec3 PhysicsSphere::getPosition() {
    return position;
}


CollisionInfo SphereSphereCollision(PhysicsSphere* sp1, PhysicsSphere* sp2) {
    CollisionInfo info;
    
    float minDistance = sp1->getRadius() + sp2->getRadius();
    float centerDistance = length((sp2->getPosition() - sp1->getPosition()));
    
    if(centerDistance < minDistance) {
        info.collision = true;
        info.collisionDepth = centerDistance - minDistance;
        info.collisionPosition = sp2->getPosition() - sp1->getPosition() - (sp2->getRadius() - info.collisionDepth);
    }
    else {
        info.collision = false;
    }
    
    return info;
}
