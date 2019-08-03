//
//  physicsSphere.hpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 28.06.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#ifndef physicsSphere_hpp
#define physicsSphere_hpp

#include <stdio.h>
#include <bullet/BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <bullet/BulletDynamics/btBulletDynamicsCommon.h>

#include "../sphere.hpp"

class PhysicsSphereData {
public:
    PhysicsSphereData();
    ~PhysicsSphereData();
    
    PhysicsSphereData(const PhysicsSphereData& other);
    PhysicsSphereData& operator=(PhysicsSphereData other);
    PhysicsSphereData(PhysicsSphereData &&other);
    
    friend void swap(PhysicsSphereData &first, PhysicsSphereData &second);
    
    btDynamicsWorld *world;
    btTransform sphereTransform;
    
    btSphereShape* sphereShape;
    btMotionState* sphereMotion;
    btRigidBody* sphereBody;
};



class PhysicsSphere: public Sphere {
public:
    PhysicsSphere(Shader *shader, const RenderData *data, btDynamicsWorld *world, float mass, btVector3 inertia = btVector3(0, 0, 0));
    ~PhysicsSphere();
    
    PhysicsSphere(const PhysicsSphere& other);
    PhysicsSphere& operator=(PhysicsSphere other);
    PhysicsSphere(PhysicsSphere &&other);
    
    friend void swap(PhysicsSphere &first, PhysicsSphere &second);
    
    void setPosition(vec3 pos);
    void setRadius(float radius);
    
    btRigidBody* getRigidBody();
    
    void render();
    
private:
    PhysicsSphereData sphereData;
    
    float mass;
    btVector3 sphereInertia;
};

#endif /* sphere_hpp */
