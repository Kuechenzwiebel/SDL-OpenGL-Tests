//
//  triangle.hpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 08.03.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#ifndef triangle_hpp
#define triangle_hpp

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <SDL2/SDL.h>

#include "arrayBuffers/arrayBuffer.hpp"
#include "arrayBuffers/dynamicArrayBuffer.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "utils.hpp"
#include "object.hpp"

using namespace glm;

class Triangle: public Object {
public:
    Triangle(Shader *shader, const RenderData *data);
    ~Triangle();
    
    void render();
    void calculateModelMat();
    
    void setPosition(vec3 position);
    void setAngle(float angle);
    void setRotationAxis(vec3 rotationAxis);
    void setSize(vec3 size);
    
    vec3 getPosition();
    float getAngle();
    vec3 getRotationAxis();
    vec3 getSize();
    
    Shader *getShaderPointer();
    
private:
    GLuint VAO;
    DynamicArrayBuffer vertex;
    ArrayBuffer texCoord;
    Shader *shader;
    const RenderData *data;
    
    
    vec3 position = vec3(0.0f);
    float angle = 0.0f;
    vec3 rotationAxis = vec3(0.0f);
    vec3 size = vec3(1.0f);
    
    mat4 model;
};

#endif /* triangle_hpp */
