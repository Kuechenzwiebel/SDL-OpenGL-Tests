//
//  camera.cpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 16.03.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#include "camera.hpp"

Camera::Camera(vec3 position, const float *deltaTime, const SDL_Event *windowEvent, bool *checkMouse):
up(vec3(0.0f, 1.0f, 0.0f)), front(vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVTY), zoom(ZOOM), yaw(YAW), pitch(PITCH),
position(position), deltaTime(deltaTime), windowEvent(windowEvent), checkMouse(checkMouse) {
    this->updateCameraVectors();
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
            this->position += this->front * velocity;
        }
        if(keystates[SDL_SCANCODE_D]) {
            this->position += this->right * velocity;
        }
        if(keystates[SDL_SCANCODE_S]) {
            this->position -= this->front * velocity;
        }
        if(keystates[SDL_SCANCODE_A]) {
            this->position -= this->right * velocity;
        }
        if(keystates[SDL_SCANCODE_SPACE]) {
            this->position.y += velocity;
        }
        if(keystates[SDL_SCANCODE_LSHIFT]) {
            this->position.y -= velocity;
        }
    }
    
    if(keystates[SDL_SCANCODE_ESCAPE]) {
        *checkMouse = false;
    }
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
