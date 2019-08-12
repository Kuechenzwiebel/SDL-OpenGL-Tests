//
//  obb.cpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 10.08.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#include "obb.hpp"

OBB::OBB(glm::vec3 middlePosition, glm::quat rotation, glm::vec3 size):
middlePosition(middlePosition), rotation(rotation), size(size) {
    type = OBB_t;
}

glm::vec3 OBB::getMiddlePosition() {
    return middlePosition;
}

glm::quat OBB::getRotation() {
    return rotation;
}

glm::vec3 OBB::getSize() {
    return size;
}


CollisionInfo obbPointCollision(OBB* obb1, glm::vec3 point) {
    CollisionInfo info;
    AABB aabb(obb1->getMiddlePosition() - obb1->getSize() / 2.0f, obb1->getMiddlePosition() + obb1->getSize() / 2.0f);
    glm::quat newRotation = obb1->getRotation();
    newRotation.w = -newRotation.w;
    glm::mat4 rotate(1), scale(1), translate(1), model(1);
    rotate = glm::rotate(glm::mat4(1), glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 1.0f));;
    scale = glm::scale(glm::mat4(1), obb1->getSize());
    translate = glm::translate(glm::mat4(1), obb1->getMiddlePosition());
    model = translate * rotate * scale;
    
    glm::vec3 newPoint = model * glm::vec4(point, 1.0f);
    /*
    printf("Min: ");
    printVec3(aabb.getP1());
    printf("Max: ");
    printVec3(aabb.getP2());
    printf("Point: ");
    printVec3(newPoint);
    printf("\n");
    */
    
//    glm::vec3 eulerAngle =
    printVec3(degrees(eulerAngles(newRotation)));
    
    info.collision = aabbPointCollision(&aabb, newPoint).collision;
    
    return info;
}
