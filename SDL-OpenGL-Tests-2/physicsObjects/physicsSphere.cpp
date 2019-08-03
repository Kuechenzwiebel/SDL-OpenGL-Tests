//
//  physicsSphere.cpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 28.06.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#include "physicsSphere.hpp"

void swap(PhysicsSphereData &first, PhysicsSphereData &second) {
    using std::swap;
    
//    if(first.sphereBody->isInWorld())
//        first.world->removeRigidBody(first.sphereBody);
//    if(second.sphereBody->isInWorld())
//        second.world->removeRigidBody(second.sphereBody);
    
    swap(first.world, second.world);
    swap(first.sphereTransform, second.sphereTransform);
    swap(first.sphereShape, second.sphereShape);
    swap(first.sphereMotion, second.sphereMotion);
    swap(first.sphereBody, second.sphereBody);
    
//    first.world->addRigidBody(first.sphereBody);
//    second.world->addRigidBody(second.sphereBody);
}

PhysicsSphereData::PhysicsSphereData():
world(nullptr), sphereShape(nullptr), sphereMotion(nullptr), sphereBody(nullptr) {
    sphereTransform.setIdentity();
}


PhysicsSphereData::~PhysicsSphereData() {
    delete sphereShape;
    delete sphereMotion;
    delete sphereBody;
    
    sphereShape = NULL;
    sphereMotion = NULL;
    sphereBody = NULL;
}


PhysicsSphereData::PhysicsSphereData(const PhysicsSphereData& other):
world(nullptr), sphereShape(nullptr), sphereMotion(nullptr), sphereBody(nullptr) {
    this->world = other.world;
    this->sphereTransform = other.sphereTransform;
    
    sphereShape = new btSphereShape(1.0f);
    *sphereShape = *other.sphereShape;
    
    sphereMotion = new btDefaultMotionState(sphereTransform);
    *sphereMotion = *other.sphereMotion;
    
    sphereBody = new btRigidBody(1.0f, sphereMotion, sphereShape);
    *sphereBody = *other.sphereBody;
}


PhysicsSphereData& PhysicsSphereData::operator=(PhysicsSphereData other) {
    swap(*this, other);
    return *this;
}

PhysicsSphereData::PhysicsSphereData(PhysicsSphereData &&other):
PhysicsSphereData() {
    swap(*this, other);
}




PhysicsSphere::PhysicsSphere(Shader *shader, const RenderData *data, btDynamicsWorld *world, float mass, btVector3 inertia):
Sphere(shader, data), mass(mass) {
    sphereData.world = world;
    
    sphereData.sphereTransform.setIdentity();
    sphereData.sphereTransform.setOrigin(btVector3(0.0f, 0.0f, 0.0f));
    sphereData.sphereShape = new btSphereShape(radius);
    sphereInertia = btVector3(100.0f, 100.0f, 100.0f);
//    sphereData.sphereShape->calculateLocalInertia(mass, sphereInertia);
//    sphereInertia += (inertia * 50.0f);
    
    sphereData.sphereMotion = new btDefaultMotionState(sphereData.sphereTransform);
    sphereData.sphereBody = new btRigidBody(mass, sphereData.sphereMotion, sphereData.sphereShape, sphereInertia);
    sphereData.sphereBody->setFriction(1.0f);
    sphereData.sphereBody->setRollingFriction(1.8f);
    sphereData.sphereBody->setSpinningFriction(0.6f);
    
    if(world != nullptr)
        sphereData.world->addRigidBody(sphereData.sphereBody);
}

void swap(PhysicsSphere &first, PhysicsSphere &second) {
    using std::swap;
    
    swap(first.VAO, second.VAO);
    swap(first.sphereVertices, second.sphereVertices);
    swap(first.sphereUVs, second.sphereUVs);
    swap(first.tex, second.tex);
    swap(first.position, second.position);
    swap(first.rotation, second.rotation);
    swap(first.radius, second.radius);
    swap(first.translate, second.translate);
    swap(first.rotate, second.rotate);
    swap(first.scale, second.scale);
    swap(first.model, second.model);
    swap(first.vertex, second.vertex);
    swap(first.colorBuffer, second.colorBuffer);
    swap(first.resolution, second.resolution);
    swap(first.shader, second.shader);
    swap(first.data, second.data);
    
    first.vertex.activate();
    first.colorBuffer.activate();
    
    second.vertex.activate();
    second.colorBuffer.activate();
    
    swap(first.mass, second.mass);
    swap(first.sphereData, second.sphereData);
}

PhysicsSphere::PhysicsSphere(const PhysicsSphere& other):
Sphere(other.Sphere::shader, other.Sphere::data), mass(0.0f) {
    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);
    int arraySize = round((360.0f / other.resolution) * (360.0f / other.resolution) * 6 * 0.5);
    sphereVertices = new vec3[arraySize];
    sphereUVs = new vec2[arraySize];
    
    std::copy(other.sphereVertices, other.sphereVertices + arraySize, sphereVertices);
    std::copy(other.sphereUVs, other.sphereUVs + arraySize, sphereUVs);
    
    tex = other.tex;
    position = other.position;
    rotation = other.rotation;
    radius = other.radius;
    translate = other.translate;
    rotate = other.rotate;
    scale = other.scale;
    vertex = other.vertex;
    colorBuffer = other.colorBuffer;
    resolution = other.resolution;
    shader = other.shader;
    data = other.data;
    
    vertex.activate();
    colorBuffer.activate();
    
    glBindVertexArray(0);
    
    mass = other.mass;
    sphereData = other.sphereData;
}

PhysicsSphere& PhysicsSphere::operator=(PhysicsSphere other) {
    swap(*this, other);
    return *this;
}

PhysicsSphere::PhysicsSphere(PhysicsSphere &&other):
PhysicsSphere(nullptr, nullptr, nullptr, 0.0f) {
    other.sphereData.world->removeRigidBody(other.sphereData.sphereBody);
    swap(*this, other);
    this->sphereData.world->addRigidBody(this->sphereData.sphereBody);
}


void PhysicsSphere::setRadius(float radius) {
    this->radius = radius;
    Sphere::setRadius(this->radius);
}

PhysicsSphere::~PhysicsSphere() {
    
}

void PhysicsSphere::setPosition(vec3 pos) {
    this->position = pos;
    sphereData.sphereTransform.setOrigin(GLMVec3ToBt(pos));
    sphereData.sphereBody->setWorldTransform(sphereData.sphereTransform);
}

void PhysicsSphere::render() {
    float modelMat[16];
    sphereData.sphereTransform = sphereData.sphereBody->getWorldTransform();
    sphereData.sphereTransform.getOpenGLMatrix(modelMat);
    setModelMat(make_mat4(modelMat));
    position = btVec3ToGLM(sphereData.sphereTransform.getOrigin());
    
    Sphere::render();
}

btRigidBody* PhysicsSphere::getRigidBody() {
    return sphereData.sphereBody;
}

