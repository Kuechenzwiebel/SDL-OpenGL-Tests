//
//  sphere.hpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 22.06.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#ifndef sphere_hpp
#define sphere_hpp

#define GLM_FORCE_SWIZZLE

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/normal.hpp>
#include <glm/gtx/norm.hpp>

#include <string>

#include "arrayBuffers/arrayBuffer.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "utils.hpp"
#include "object.hpp"

const float sphereResolution = 2.5f;
///sphereArraySize can be calculated using this formula: round((360.0f / sphereResolution) * (360.0f / sphereResolution) * 6 * 0.5)
const int sphereArraySize = 62208;


class Sphere: public Object {
public:
    Sphere(Shader *shader, const RenderData *data, bool *wireframe = nullptr);
    ~Sphere();
    
    void render();
    
    void setPosition(glm::vec3 position);
    void setRadius(float radius);
    void setRotation(glm::vec4 rotation);
    void setModelMat(glm::mat4 modelMat);
    
    void setTexture(Texture *texture);
    
    glm::vec3 getPosition();
    glm::vec4 getRotation();
    float getRadius();
    glm::vec3 getRealPosition();
    
    Shader *getShaderPointer();
    
private:
    GLuint VAO;
    ArrayBuffer vertex, colorBuffer, normal;
    
    Shader *shader;
    const RenderData *data;
    
    Texture *tex;
    
    bool *wireframe;
    
    
    glm::vec3 position;
    glm::vec4 rotation;
    float radius;
    
    glm::mat4 translate, rotate, scale, model;
    
    
    static glm::vec3 sphereVertices[sphereArraySize];
    static glm::vec2 sphereUVs[sphereArraySize];
    static glm::vec3 sphereNormals[sphereArraySize];
    
};

#endif /* sphere_hpp */
