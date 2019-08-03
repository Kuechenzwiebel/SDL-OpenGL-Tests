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

using namespace glm;


const float YAW        = -90.0f;
const float PITCH      =  0.0f;
const float SPEED      =  8.0f;
//const float SPEED      =  3000000.0f;
const float SENSITIVTY =  0.25f;
const float ZOOM       =  45.0f;

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
    
private:
    vec3 position;
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
};

#endif /* camera_hpp */
