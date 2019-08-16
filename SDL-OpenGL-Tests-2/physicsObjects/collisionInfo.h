//
//  collisionInfo.h
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 05.08.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#ifndef collisionInfo_h
#define collisionInfo_h

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>

struct CollisionInfo {
    bool collision;
    float collisionDepth;
    glm::vec3 collisionPosition;
};

#endif /* collisionInfo_h */
