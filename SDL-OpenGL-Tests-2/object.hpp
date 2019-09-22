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

class Object {
public:
    Object();
    virtual ~Object();
    
    virtual void render();
    virtual glm::vec3 getPosition();
    virtual glm::vec3 getRealPosition();
    virtual glm::vec3 getSize();
    virtual void setPosition(glm::vec3 position);
    virtual Shader* getShaderPointer();
};

bool operator<(std::pair<float, Object*> l, std::pair<float, Object*> r);
bool operator>(std::pair<float, Object*> l, std::pair<float, Object*> r);

#endif /* object_hpp */
