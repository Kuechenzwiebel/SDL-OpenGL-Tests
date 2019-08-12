//
//  aabb.hpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 10.08.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#ifndef aabb_hpp
#define aabb_hpp

#include <glm/glm.hpp>
#include <stdio.h>

#include "collisionInfo.h"
#include "physicsObject.hpp"

class AABB: public PhysicsObject {
public:
    AABB(glm::vec3 p1, glm::vec3 p2);
    
    glm::vec3 getP1();
    glm::vec3 getP2();
    
private:
    glm::vec3 p1, p2;
};

CollisionInfo aabbPointCollision(AABB* aabb1, glm::vec3 point);

#endif /* aabb_hpp */
