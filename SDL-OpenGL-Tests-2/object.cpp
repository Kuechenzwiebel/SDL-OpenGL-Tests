//
//  object.cpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 19.06.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#include "object.hpp"

Object::Object() {
    
}

void Object::render() {
    
}

vec3 Object::getPosition() {
    return vec3(0.0f);
}

vec3 Object::getSize() {
    return vec3(0.0f);
}

void Object::setPosition(vec3 position) {

}

Shader* Object::getShaderPointer() {
    return nullptr;
}

bool operator<(std::pair<float, Object*> l, std::pair<float, Object*> r) {
    if(l.first < r.first) {
        return true;
    }
    else {
        return false;
    }
}

bool operator>(std::pair<float, Object*> l, std::pair<float, Object*> r) {
    if(l.first > r.first) {
        return true;
    }
    else {
        return false;
    }
}
