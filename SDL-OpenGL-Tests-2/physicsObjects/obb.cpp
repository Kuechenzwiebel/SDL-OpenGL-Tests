//
//  obb.cpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 10.08.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#include "obb.hpp"

OBB::OBB(vec3 middlePosition, quat rotation, vec3 size):
middlePosition(middlePosition), rotation(rotation), size(size) {
    type = OBB_t;
}

vec3 OBB::getMiddlePosition() {
    return middlePosition;
}

quat OBB::getRotation() {
    return rotation;
}

vec3 OBB::getSize() {
    return size;
}


CollisionInfo obbPointCollision(OBB* obb1, vec3 point) {
    CollisionInfo info;
    AABB aabb(obb1->getMiddlePosition() - obb1->getSize() / 2.0f, obb1->getMiddlePosition() + obb1->getSize() / 2.0f);
    quat newRotation = obb1->getRotation();
    newRotation.w = -newRotation.w;
    mat4 rotate(1), scale(1), translate(1), model(1);
    rotate = toMat4(newRotation);
    scale = glm::scale(mat4(1), obb1->getSize());
    translate = glm::translate(mat4(1), obb1->getMiddlePosition());
    model = translate * rotate * scale;
    
    vec3 newPoint = model * vec4(point, 1.0f);
    /*
    printf("Min: ");
    printVec3(aabb.getP1());
    printf("Max: ");
    printVec3(aabb.getP2());
    printf("Point: ");
    printVec3(newPoint);
    printf("\n");
    */
    
//    vec3 eulerAngle =
    printVec3(degrees(eulerAngles(newRotation)));
    
    info.collision = aabbPointCollision(&aabb, newPoint).collision;
    
    return info;
}
