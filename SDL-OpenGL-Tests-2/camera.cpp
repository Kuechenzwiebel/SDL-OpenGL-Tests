//
//  camera.cpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 16.03.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#include "camera.hpp"

Camera::Camera(vec3 position, const float *deltaTime, const SDL_Event *windowEvent, bool *checkMouse):
up(vec3(0.0f, 1.0f, 0.0f)), front(vec3(0.0f, 0.0f, -1.0f)), movementSpeed(1.388f * 1.0f), mouseSensitivity(0.25f), zoom(45.0f), yaw(0.0f), pitch(0.0f),
position(position), theoreticalPosition(position), deltaTime(deltaTime), windowEvent(windowEvent), checkMouse(checkMouse), noise(420) {
    this->updateCameraVectors();
}

void Camera::updateCameraVectors() {
    front.x = cos(radians(this->yaw)) * cos(radians(this->pitch));
    front.y = sin(radians(this->pitch));
    front.z = sin(radians(this->yaw)) * cos(radians(this->pitch));
    
    this->front = normalize(front);
    this->right = normalize(cross(this->front, this->up));
}

void Camera::processInput() {
    const Uint8 *keystates = SDL_GetKeyboardState(NULL);
    float velocity = this->movementSpeed * *deltaTime;
    
    if(*checkMouse) {
        if(keystates[SDL_SCANCODE_W]) {
            this->theoreticalPosition += this->front * velocity;
        }
        if(keystates[SDL_SCANCODE_D]) {
            this->theoreticalPosition += this->right * velocity;
        }
        if(keystates[SDL_SCANCODE_S]) {
            this->theoreticalPosition -= this->front * velocity;
        }
        if(keystates[SDL_SCANCODE_A]) {
            this->theoreticalPosition -= this->right * velocity;
        }
        if(keystates[SDL_SCANCODE_SPACE]) {
            this->theoreticalPosition.y += velocity;
        }
        if(keystates[SDL_SCANCODE_LSHIFT]) {
            this->theoreticalPosition.y -= velocity;
        }
    }
    
    if(keystates[SDL_SCANCODE_ESCAPE]) {
        *checkMouse = false;
    }
    
    bool collisionHappend = false;
    
    float freq = 4.0f, multiplier = 2.0f;
    int octaves = 10;
    
    if(!collisionHappendLastFrame) {
        theoreticalPosition.y -= 1.0f * *deltaTime;
    }
    
    float mapPosition = (noise.perl(theoreticalPosition.x, theoreticalPosition.z, freq, octaves) * multiplier) - 2.0f + 0.2f;
    
    
    if(theoreticalPosition.y < mapPosition) {
        theoreticalPosition.y = mapPosition;
//        collisionHappend = true;
    }
    
    
    if(!collisionHappend) {
        for(int i = 0; i < objects->size(); i++) {/*
                                                   switch ((*objects)[i]->getType()) {
                                                   case SPHERE:*/
            if(spherePointCollision((PhysicsSphere *)((*objects)[i]), theoreticalPosition).collision) {
                collisionHappend = true;
                break;
            }
        }
    }
   
    
    if(collisionHappend) {
        theoreticalPosition = position;
    }
    else {
        position = theoreticalPosition;
    }
    collisionHappendLastFrame = collisionHappend;
//    position = theoreticalPosition;
//    position.y = mapPosition + 0.2f;
}

void Camera::processMouseInput() {
    if(*checkMouse) {
        if((*windowEvent).type == SDL_MOUSEMOTION) {
            this->yaw += (*windowEvent).motion.xrel * this->mouseSensitivity;
            this->pitch -= (*windowEvent).motion.yrel * this->mouseSensitivity;
            
            if (this->pitch > 89.0f) {
                this->pitch = 89.0f;
            }
            
            if (this->pitch < -89.0f) {
                this->pitch = -89.0f;
            }
            
            this->updateCameraVectors();
        }
    }
    
    yaw = fmod(yaw + 360.0f, 360.0f);
}

float Camera::getMouseSensitivity() {
    return mouseSensitivity;
}

void Camera::setMouseSensitivity(float sensitivity) {
    if(sensitivity > 0.0f) {
        mouseSensitivity = sensitivity;
    }
}

mat4 Camera::getViewMatrix() {
    return lookAt(this->position, this->position + this->front, this->up);
}

float Camera::getZoom() {
    return this->zoom;
}

vec3 Camera::getPosition() {
    return this->position;
}

vec3 Camera::getFront() {
    return this->front;
}

void Camera::setPosition(vec3 pos) {
    this->position = pos;
}

float *Camera::getYawPointer() {
    return &yaw;
}

float *Camera::getPitchPointer() {
    return &pitch;
}

void Camera::setCollisonObjectsPointer(std::vector<PhysicsSphere*> *objects) {
    this->objects = objects;
}
