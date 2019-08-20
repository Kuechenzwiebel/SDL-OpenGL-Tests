//
//  ray.hpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 16.08.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#ifndef ray_hpp
#define ray_hpp

#define GLM_FORCE_SWIZZLE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <stdio.h>

#include "../utils.hpp"

class Ray {
public:
    Ray(glm::vec3 rayStartPosition, glm::vec3 rayDirection, float stepSize);
    
    void step();
    void reset();
    
    void setRayStartPosition(glm::vec3 position);
    void setRayDirection(glm::vec3 direction);
    
    glm::vec3 getRayPosition();
    
private:
    glm::vec3 rayStartPosition;
    glm::vec3 rayDirection;
    glm::vec3 rayPosition;
    glm::vec3 stepDirection;
    float stepSize;
};

#endif /* ray_hpp */
