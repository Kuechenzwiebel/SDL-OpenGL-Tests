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

class Rectangle: public Object {
public:
    Rectangle(Shader *shader, const RenderData *data);
    ~Rectangle();
    
    void render();
    
    void setPosition(glm::vec3 position);
    void setSize(glm::vec3 size);
    void setTexture(Texture texture);
    void setRotation(glm::quat rotation);
    void addRotation(glm::quat rotation);
    
    glm::vec3 getPosition();
    glm::vec3 getSize();
    glm::quat getRotation();
    
    Shader *getShaderPointer();
    
private:
    GLuint VAO;
    ArrayBuffer vertex;
    ArrayBuffer texCoord;
    Texture tex;
    
    Shader *shader;
    const RenderData *data;
    
    glm::vec3 position;
    glm::vec3 size;
    glm::quat rotation;
    
    glm::mat4 translate, rotate, scale, model;
};

#endif /* rectangle_hpp */
