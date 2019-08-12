//
//  aabb.cpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 10.08.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#include "aabb.hpp"

AABB::AABB(glm::vec3 p1, glm::vec3 p2):
p1(min(p1, p2)), p2(max(p1, p2)) {
    type = AABB_t;
}

glm::vec3 AABB::getP1() {
    return p1;
}

glm::vec3 AABB::getP2() {
    return p2;
}

CollisionInfo aabbPointCollision(AABB* aabb1, glm::vec3 point) {
    CollisionInfo info;
    if(point.x >= aabb1->getP1().x && point.x <= aabb1->getP2().x &&
       point.y >= aabb1->getP1().y && point.y <= aabb1->getP2().y &&
       point.z >= aabb1->getP1().z && point.z <= aabb1->getP2().z) {
        info.collision = true;
    }
    else {
        info.collision = false;
    }
    return info;
}
