//
//  camera.cpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 16.03.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#include "camera.hpp"

Camera::Camera(glm::vec3 position, const float *deltaTime, const SDL_Event *windowEvent, bool *checkMouse):
up(glm::vec3(0.0f, 1.0f, 0.0f)), front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(1.388f * 1.0f), mouseSensitivity(0.25f), zoom(45.0f), yaw(0.0f), pitch(0.0f),
position(position), theoreticalPosition(position), deltaTime(deltaTime), windowEvent(windowEvent), checkMouse(checkMouse) {
    this->updateCameraVectors();
}

void Camera::updateCameraVectors() {
    front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    front.y = sin(glm::radians(this->pitch));
    front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    
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
    
    collisionHappend = false;
    
    if(!collisionHappend) {
//        theoreticalPosition.y -= 1.0f * *deltaTime;
    }
    
    float mapPosition = (info.noise->perl(theoreticalPosition.x, theoreticalPosition.z, info.freq, info.octaves) * info.multiplier) - 2.0f + 0.2f;
    
    if(theoreticalPosition.x < info.width / 2.0f && theoreticalPosition.x > -(info.width / 2.0f) && theoreticalPosition.y < info.width / 2.0f && theoreticalPosition.y > -(info.width / 2.0f)) {
        if(theoreticalPosition.y < mapPosition) {
            theoreticalPosition.y = mapPosition;
        }
    }
    
    if(!collisionHappend) {
        for(int i = 0; i < objects->size(); i++) {
            switch ((*objects)[i]->getObjectType()) {
                case SPHERE_t:
                    if(spherePointCollision((PhysicsSphere*)((*objects)[i]), theoreticalPosition).collision) {
                        collisionHappend = true;
                    }
                    break;
                    
                case AABB_t:
                    if(aabbPointCollision((AABB*)((*objects)[i]), theoreticalPosition).collision) {
                        collisionHappend = true;
                    }
                    break;
                    
                case OBB_t:
                    if(obbPointCollision((OBB*)((*objects)[i]), theoreticalPosition).collision) {
                        collisionHappend = true;
                    }
                    break;
                    
                default:
                    break;
            }
            if(collisionHappend) {
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

void Camera::setCollisonObjectsPointer(std::vector<PhysicsObject*> *objects) {
    this->objects = objects;
}

void Camera::setPerlinMapInfo(PerlinMapInformation info) {
    this->info = info;
}
