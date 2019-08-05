//
//  physicsSphere.hpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 05.08.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#ifndef physicsSphere_hpp
#define physicsSphere_hpp

#include <glm/glm.hpp>
#include <stdio.h>

#include "collisionInfo.h"

using namespace glm;

class PhysicsSphere {
public:
    PhysicsSphere(float radius, vec3 position);
    
    float getRadius();
    vec3 getPosition();
    
private:
    float radius;
    vec3 position;
};



CollisionInfo SphereSphereCollision(PhysicsSphere* sp1, PhysicsSphere* sp2);

#endif /* physicsSphere_hpp */
