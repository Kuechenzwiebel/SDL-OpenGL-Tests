//
//  physicsObject.hpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 08.08.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#ifndef physicsObject_hpp
#define physicsObject_hpp

#include <stdio.h>

enum PhysicsObjectTypes {
    SPHERE_t,
    AABB_t,
    OBB_t
};

class PhysicsObject {
public:
    PhysicsObject();
    
    PhysicsObjectTypes getObjectType();
    
protected:
    PhysicsObjectTypes type;
};

#endif /* physicsObject_hpp */
