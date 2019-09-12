//
//  cube.hpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 16.04.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#ifndef cube_hpp
#define cube_hpp

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

class Cube: public Object {
public:
    Cube(Shader *shader, const RenderData *data, bool *wireframe = nullptr);
    ~Cube();
    
    void render();
    
    void setTexture(Texture *texture);
    void setCubemap(Cubemap *texture);
    
    void setPosition(glm::vec3 position);
    void setSize(glm::vec3 size);
    void setRotation(glm::vec4 rotation);
    void setModelMat(glm::mat4 modelMat);
    
    glm::vec3 getPosition();
    glm::vec4 getRotation();
    glm::vec3 getSize();
    
    Shader *getShaderPointer();
    
private:
    GLuint VAO;
    ArrayBuffer vertex, texCoord, normals;
    Texture *tex;
    Cubemap *map;
    
    Shader *shader;
    const RenderData *data;
    
    bool *wireframe;
    
    
    glm::vec3 position;
    glm::vec4 rotation;
    glm::vec3 size;
    
    glm::mat4 translate, rotate, scale, model;
};

#endif /* cube_hpp */
