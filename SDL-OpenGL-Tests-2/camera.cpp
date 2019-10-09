//
//  camera.cpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 16.03.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#include "camera.hpp"

Camera::Camera(const float *deltaTime, const SDL_Event *windowEvent, bool *checkMouse):
up(glm::vec3(0.0f, 1.0f, 0.0f)), front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(0.6f * 100.0f), mouseSensitivity(0.25f), zoom(45.0f), yaw(0.0f), pitch(0.0f),
footPosition(glm::vec3(0.0f, 20.0f, 0.0f)), theoreticalFootPosition(glm::vec3(0.0f, 20.0f, 0.0f)), eyePosition(glm::vec3(0.0f, 20.0f, 0.0f) + glm::vec3(0.0f, 1.73f, 0.0f)), deltaTime(deltaTime), windowEvent(windowEvent), checkMouse(checkMouse), gravity(true), inVehicle(false), timeSinceLastOnFloor(0) {
    this->updateCameraVectors();
}

void Camera::updateCameraVectors() {
    front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    front.y = sin(glm::radians(this->pitch));
    front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    
    this->front = glm::normalize(front);
    this->right = glm::normalize(cross(this->front, this->up));
}

void Camera::processInput() {
    const Uint8 *keystates = SDL_GetKeyboardState(NULL);
    float velocity = this->movementSpeed * *deltaTime;
    
    if(!inVehicle) {
        if(*checkMouse) {
            if(keystates[SDL_SCANCODE_W])
                this->theoreticalFootPosition += this->front * velocity;
            if(keystates[SDL_SCANCODE_D])
                this->theoreticalFootPosition += this->right * velocity;
            if(keystates[SDL_SCANCODE_S])
                this->theoreticalFootPosition -= this->front * velocity;
            if(keystates[SDL_SCANCODE_A])
                this->theoreticalFootPosition -= this->right * velocity;
            if(keystates[SDL_SCANCODE_SPACE])
                this->theoreticalFootPosition.y += velocity;
            if(keystates[SDL_SCANCODE_LSHIFT])
                this->theoreticalFootPosition.y -= velocity;
        }
        
        collisionHappend = false;
        mapCollisionHappend = false;
        
        if(gravity)
            theoreticalFootPosition.y -= (9.8f * (SDL_GetTicks() - timeSinceLastOnFloor) / 1000.0f) * *deltaTime;
        
        float mapPosition = 0.0f;
        if(noise != nullptr) {
            mapPosition = noise->octaveNoise(theoreticalFootPosition.x, theoreticalFootPosition.z) - 2.0f;
        }
        
        if(theoreticalFootPosition.y < mapPosition) {
            theoreticalFootPosition.y = mapPosition;
            mapCollisionHappend = true;
        }
        
        if(!collisionHappend)
            collisionHappend = worldPointCollision(objects, theoreticalFootPosition);
    }
    
    if(collisionHappend) {
        theoreticalFootPosition = footPosition;
    }
    else {
        footPosition = theoreticalFootPosition;
    }
    
    if((collisionHappend || mapCollisionHappend) && gravity)
        timeSinceLastOnFloor = SDL_GetTicks();
    
    
    if(!gravity)
        timeSinceLastOnFloor = SDL_GetTicks();
    
    
    if(!inVehicle)
        eyePosition = footPosition + glm::vec3(0.0f, 1.73f, 0.0f);
    else
        eyePosition = footPosition;
}

void Camera::processMouseInput() {
    if(*checkMouse) {
        if(windowEvent->type == SDL_MOUSEMOTION) {
            this->yaw += windowEvent->motion.xrel * this->mouseSensitivity;
            this->pitch -= windowEvent->motion.yrel * this->mouseSensitivity;
            
            if (this->pitch >= 90.0f) {
                this->pitch = 89.99f;
            }
            
            if (this->pitch <= -90.0f) {
                this->pitch = -89.99f;
            }
            
            this->updateCameraVectors();
        }
    }
    
    if(windowEvent->type == SDL_KEYDOWN && windowEvent->key.keysym.sym == SDLK_g) {
        swapBool(&gravity);
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

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(this->eyePosition, this->eyePosition + this->front, this->up);
}

float Camera::getZoom() {
    return this->zoom;
}

glm::vec3 Camera::getPosition() {
    return this->footPosition;
}

glm::vec3 Camera::getFront() {
    return this->front;
}

void Camera::setPosition(glm::vec3 pos) {
    this->footPosition = pos;
    this->theoreticalFootPosition = pos;
}

float *Camera::getYawPointer() {
    return &yaw;
}

float *Camera::getPitchPointer() {
    return &pitch;
}

void Camera::setMapNoise(PerlinNoise *noise) {
    this->noise = noise;
}
