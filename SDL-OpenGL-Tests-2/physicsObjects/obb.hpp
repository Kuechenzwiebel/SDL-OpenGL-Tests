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

class OBB: public PhysicsObject {
public:
    OBB(glm::vec3 middlePosition, glm::vec4 rotation, glm::vec3 size);
    
    glm::vec3 getMiddlePosition();
    glm::vec4 getRotation();
    glm::vec3 getSize();
    
private:
    glm::vec3 middlePosition;
    glm::vec4 rotation;
    glm::vec3 size;
};

CollisionInfo obbPointCollision(OBB* obb1, glm::vec3 point);

#endif /* obb_hpp */
