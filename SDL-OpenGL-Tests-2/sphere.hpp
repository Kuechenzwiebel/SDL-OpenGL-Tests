//
//  sphere.hpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 22.06.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#ifndef sphere_hpp
#define sphere_hpp

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

class Sphere: public Object {
public:
    Sphere(Shader *shader, const RenderData *data);
    ~Sphere();
    
    Sphere(const Sphere& other);
    Sphere& operator=(Sphere other);
    Sphere(Sphere &&other);
    
    friend void swap(Sphere &first, Sphere &second);
    
    void render();
    
    void setPosition(glm::vec3 position);
    void setRadius(float radius);
    void setRotation(glm::quat rotation);
    void addRotation(glm::quat rotation);
    void setModelMat(glm::mat4 model);
    
    void setTexture(Texture texture);
    
    glm::vec3 getPosition();
    glm::quat getRotation();
    float getRadius();
    
    Shader *getShaderPointer();
    
protected:
    Texture tex;
    
    glm::vec3 position;
    glm::quat rotation;
    float radius;
    
    glm::mat4 translate, rotate, scale, model;
    
    GLuint VAO;
    ArrayBuffer vertex, colorBuffer;
    
    
    glm::vec3 *sphereVertices;
    glm::vec2 *sphereUVs;
    
    float resolution;
    
    Shader *shader;
    const RenderData *data;
};

#endif /* sphere_hpp */
