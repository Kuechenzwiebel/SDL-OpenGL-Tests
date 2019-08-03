//
//  cube.hpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 16.04.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#ifndef cube_hpp
#define cube_hpp

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/normal.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <string>
#include <SDL2/SDL.h>

#include "arrayBuffers/arrayBuffer.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "cubemap.hpp"
#include "utils.hpp"
#include "object.hpp"

using namespace glm;

class Cube: public Object {
public:
    Cube(Shader *shader, const RenderData *data);
    ~Cube();
    
    void render();
    
    void setTexture(Texture texture);
    void setCubemap(Cubemap texture);
    
    void setPosition(vec3 position);
    void setSize(vec3 size);
    void setRotation(quat rotation);
    void addRotation(quat rotation);
    
    vec3 getPosition();
    quat getRotation();
    vec3 getSize();
    
    Shader *getShaderPointer();
    
private:
    GLuint VAO;
    ArrayBuffer vertex, texCoord, normals;
    Texture tex;
    Cubemap map;
    
    Shader *shader;
    const RenderData *data;
    
    vec3 position;
    quat rotation;
    vec3 size;
    
    mat4 translate, rotate, scale, model;
};

#endif /* cube_hpp */
