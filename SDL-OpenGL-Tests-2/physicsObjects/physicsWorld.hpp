//
//  physicsWorld.hpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 16.08.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#ifndef physicsWorld_hpp
#define physicsWorld_hpp

#include <stdio.h>
#include <vector>

#include "physicsObject.hpp"

class PhysicsWorld {
public:
    PhysicsWorld();
    
    std::vector<PhysicsObject *> objects;
    
    
};

#endif /* physicsWorld_hpp */
