//
//  rectangle.hpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 17.03.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#ifndef rectangle_hpp
#define rectangle_hpp

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <string>
#include <SDL2/SDL.h>

#include "arrayBuffers/arrayBuffer.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "utils.hpp"
#include "object.hpp"

using namespace glm;

class Rectangle: public Object {
public:
    Rectangle(Shader *shader, const RenderData *data);
    ~Rectangle();
    
    void render();
    
    void setPosition(vec3 position);
    void setSize(vec3 size);
    void setTexture(Texture texture);
    void setRotation(quat rotation);
    void addRotation(quat rotation);
    
    vec3 getPosition();
    vec3 getSize();
    quat getRotation();
    
    Shader *getShaderPointer();
    
private:
    GLuint VAO;
    ArrayBuffer vertex;
    ArrayBuffer texCoord;
    Texture tex;
    
    Shader *shader;
    const RenderData *data;
    
    vec3 position;
    vec3 size;
    quat rotation;
    
    mat4 translate, rotate, scale, model;
};

#endif /* rectangle_hpp */
