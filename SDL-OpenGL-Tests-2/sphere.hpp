//
//  sphere.hpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 22.06.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#ifndef sphere_hpp
#define sphere_hpp

#define GLM_SWIZZLE

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <SDL2/SDL.h>

#include "arrayBuffers/arrayBuffer.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "utils.hpp"
#include "object.hpp"

const float sphereResolution = 10.0f;
///sphereArraySize can be calculated using this formula: round((360.0f / sphereResolution) * (360.0f / sphereResolution) * 6 * 0.5)
const int sphereArraySize = 3888;


class Sphere: public Object {
public:
    Sphere(Shader *shader, const RenderData *data);
    ~Sphere();
    
    void render();
    
    void setPosition(glm::vec3 position);
    void setRadius(float radius);
    void setRotation(glm::vec4 rotation);
    void setModelMat(glm::mat4 model);
    
    void setTexture(Texture texture);
    
    glm::vec3 getPosition();
    glm::vec4 getRotation();
    float getRadius();
    
    Shader *getShaderPointer();
    
protected:
    Texture tex;
    
    glm::vec3 position;
    glm::vec4 rotation;
    float radius;
    
    glm::mat4 translate, rotate, scale, model;
    
    GLuint VAO;
    ArrayBuffer vertex, colorBuffer;
    
    glm::vec3 sphereVertices[sphereArraySize];
    glm::vec2 sphereUVs[sphereArraySize];
    
    Shader *shader;
    const RenderData *data;
};

#endif /* sphere_hpp */
