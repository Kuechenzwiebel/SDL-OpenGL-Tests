//
//  physicsWorld.cpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 16.08.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#include "physicsWorld.hpp"

PhysicsWorld::PhysicsWorld() {

}
    
bool worldPointCollision(PhysicsWorld *world, glm::vec3 point) {
    bool collision = false;
    
    for(unsigned int i = 0; i < world->objects.size(); i++) {
        switch (world->objects[i]->getObjectType()) {
            case SPHERE_t:
                if(spherePointCollision((PhysicsSphere*)world->objects[i], point).collision) {
                    collision = true;
                }
                break;
                
            case AABB_t:
                if(aabbPointCollision((AABB*)world->objects[i], point).collision) {
                    collision = true;
                }
                break;
                
            case OBB_t:
                if(obbPointCollision((OBB*)world->objects[i], point).collision) {
                    collision = true;
                }
                break;
                
            default:
                break;
        }
        if(collision) {
            break;
        }
    }
    
    return collision;
}
