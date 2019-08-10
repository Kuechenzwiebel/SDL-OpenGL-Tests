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

using namespace glm;

class AABB: public PhysicsObject {
public:
    AABB(vec3 p1, vec3 p2);
    
    vec3 getP1();
    vec3 getP2();
    
private:
    vec3 p1, p2;
};

CollisionInfo aabbPointCollision(AABB* aabb1, vec3 point);

#endif /* aabb_hpp */
