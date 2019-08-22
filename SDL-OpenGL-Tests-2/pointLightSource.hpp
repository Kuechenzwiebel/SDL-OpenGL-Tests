//
//  pointLightSource.hpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 24.07.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#ifndef pointLightSource_hpp
#define pointLightSource_hpp

#define GLM_FORCE_SWIZZLE

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/normal.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

#include "arrayBuffers/arrayBuffer.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "cubemap.hpp"
#include "utils.hpp"
#include "object.hpp"

class PointLightSource: public Object {
public:
    PointLightSource(Object *renderObject, Shader *shaders, int shaderAmount);
    
    void render();
    
    void setPosition(glm::vec3 position);
    glm::vec3 getPosition();
    
private:
    Object *renderObject;
    Shader *shaders;
    int shaderAmount;
    
    glm::vec3 position;
};

#endif /* pointLightSource_hpp */
