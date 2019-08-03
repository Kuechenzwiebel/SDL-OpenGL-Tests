//
//  object.hpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 19.06.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#ifndef object_hpp
#define object_hpp

#include <stdio.h>
#include <utility>
#include <glm/glm.hpp>

#include "shader.hpp"

using namespace glm;

class Object {
public:
    Object();
    
    virtual void render();
    virtual vec3 getPosition();
    virtual vec3 getSize();
    virtual void setPosition(vec3 position);
    virtual Shader* getShaderPointer();
};

bool operator<(std::pair<float, Object*> l, std::pair<float, Object*> r);
bool operator>(std::pair<float, Object*> l, std::pair<float, Object*> r);

#endif /* object_hpp */
