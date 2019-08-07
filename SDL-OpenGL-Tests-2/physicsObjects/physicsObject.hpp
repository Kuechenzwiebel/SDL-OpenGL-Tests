//
//  physicsObject.hpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 05.08.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#ifndef physicsObject_hpp
#define physicsObject_hpp

#include <glm/glm.hpp>
#include <stdio.h>

using namespace glm;

enum ObjectTypes {
    NOTHING,
    SPHERE
};

class PhysicsObject {
public:
    PhysicsObject();
    
//    virtual ObjectTypes getType();
    
protected:
//    ObjectTypes type;
};

#endif /* physicsObject_hpp */
