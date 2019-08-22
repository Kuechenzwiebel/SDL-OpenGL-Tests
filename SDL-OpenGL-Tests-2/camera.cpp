//
//  camera.cpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 16.03.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#include "camera.hpp"

Camera::Camera(glm::vec3 position, const float *deltaTime, const SDL_Event *windowEvent, bool *checkMouse):
up(glm::vec3(0.0f, 1.0f, 0.0f)), front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(1.388f * 1.5f), mouseSensitivity(0.25f), zoom(45.0f), yaw(0.0f), pitch(0.0f),
position(position), theoreticalPosition(position), deltaTime(deltaTime), windowEvent(windowEvent), checkMouse(checkMouse) {
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
    
    collisionHappend = false;
    
    if(!collisionHappend) {
        theoreticalPosition.y -= 1.0f * *deltaTime;
    }
    
    float mapPosition = 0.0f;
    if(info.noise != nullptr && theoreticalPosition.x < info.width / 2.0f && theoreticalPosition.x > -(info.width / 2.0f) && theoreticalPosition.z < info.width / 2.0f && theoreticalPosition.z > -(info.width / 2.0f)) {
        mapPosition = (info.noise->perl(theoreticalPosition.x, theoreticalPosition.z, info.freq, info.octaves) * info.multiplier) - 2.0f + 0.2f;
    }
    
    if(theoreticalPosition.y < mapPosition) {
        theoreticalPosition.y = mapPosition;
    }
    
    if(!collisionHappend) {
        collisionHappend = worldPointCollision(objects, theoreticalPosition);
    }
   
    
    if(collisionHappend) {
        theoreticalPosition = position;
    }
    else {
        position = theoreticalPosition;
    }
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
    return glm::lookAt(this->position, this->position + this->front, this->up);
}

float Camera::getZoom() {
    return this->zoom;
}

glm::vec3 Camera::getPosition() {
    return this->position;
}

glm::vec3 Camera::getFront() {
    return this->front;
}

void Camera::setPosition(glm::vec3 pos) {
    this->position = pos;
}

float *Camera::getYawPointer() {
    return &yaw;
}

float *Camera::getPitchPointer() {
    return &pitch;
}

void Camera::setPerlinMapInfo(PerlinMapInformation info) {
    this->info = info;
}
