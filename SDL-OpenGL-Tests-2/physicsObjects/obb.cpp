//
//  obb.cpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 10.08.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#include "obb.hpp"

OBB::OBB(glm::vec3 middlePosition, glm::vec4 rotation, glm::vec3 size):
middlePosition(middlePosition), rotation(rotation), size(size) {
    type = OBB_t;
}

glm::vec3 OBB::getMiddlePosition() {
    return middlePosition;
}

glm::vec4 OBB::getRotation() {
    return rotation;
}

glm::vec3 OBB::getSize() {
    return size;
}


CollisionInfo obbPointCollision(OBB* obb1, glm::vec3 point) {
    CollisionInfo info;
    AABB aabb(obb1->getMiddlePosition() - obb1->getSize() / 2.0f, obb1->getMiddlePosition() + obb1->getSize() / 2.0f);
    
    glm::mat4 rotate(1), translate(1);
    rotate = glm::rotate(glm::mat4(1), -obb1->getRotation().w, obb1->getRotation().xyz());
    translate = glm::translate(glm::mat4(1), obb1->getMiddlePosition());
    
    glm::vec3 newPoint = translate * rotate * glm::vec4(point - obb1->getMiddlePosition(), 1.0f);
//    newPoint = newPoint + point;
    
    info = aabbPointCollision(&aabb, newPoint);
    
    printf("P1: ");
    printVec3(aabb.getP1());
    printf("P2: ");
    printVec3(aabb.getP2());
    printf("P0: ");
    printVec3(newPoint);
    printf("\n");
    
    return info;
}
