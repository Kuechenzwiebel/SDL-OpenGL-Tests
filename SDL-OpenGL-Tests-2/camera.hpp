//
//  camera.hpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 16.03.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#ifndef camera_hpp
#define camera_hpp

#include <vector>
#include <iostream>
#include <cmath>

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL2/SDL.h>

#include "physicsObjects/physicsObject.hpp"
#include "physicsObjects/physicsSphere.hpp"
#include "physicsObjects/aabb.hpp"
#include "physicsObjects/obb.hpp"

#include "perlinMap.hpp"

using namespace glm;

enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Camera {
public:
    Camera(vec3 position, const float *deltaTime, const SDL_Event *windowEvent, bool *checkMouse);
    
    mat4 getViewMatrix();
    float getZoom();
    vec3 getPosition();
    vec3 getFront();
    float getMouseSensitivity();
    
    void setPosition(glm::vec3 position);
    void setMouseSensitivity(float sensitivity);
    
    float *getYawPointer();
    float *getPitchPointer();
    
    void processMouseInput();
    void processInput();
    
    void setCollisonObjectsPointer(std::vector<PhysicsObject*> *objects);
    void setPerlinMapInfo(PerlinMapInformation info);
    
private:
    vec3 position, theoreticalPosition;
    vec3 front;
    vec3 up;
    vec3 right;
    
    const float *deltaTime;
    const SDL_Event *windowEvent;
    bool *checkMouse;
    
    float yaw;
    float pitch;
    
    float movementSpeed;
    float mouseSensitivity;
    float zoom;
    
    void updateCameraVectors();
    
    std::vector<PhysicsObject*> *objects;
    PerlinMapInformation info;
    bool collisionHappend;
    bool collisionHappendLastFrame;
};

#endif /* camera_hpp */
