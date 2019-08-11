//
//  obb.hpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 10.08.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#ifndef obb_hpp
#define obb_hpp

#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <stdio.h>

#include "collisionInfo.h"
#include "physicsObject.hpp"
#include "aabb.hpp"

#include "../utils.hpp"

using namespace glm;

class OBB: public PhysicsObject {
public:
    OBB(vec3 middlePosition, quat rotation, vec3 size);
    
    vec3 getMiddlePosition();
    quat getRotation();
    vec3 getSize();
    
private:
    vec3 middlePosition;
    quat rotation;
    vec3 size;
};

CollisionInfo obbPointCollision(OBB* obb1, vec3 point);

#endif /* obb_hpp */
