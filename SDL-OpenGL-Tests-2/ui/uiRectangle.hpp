//
//  rectangle.hpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 30.03.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#ifndef ui_rectangle_hpp
#define ui_rectangle_hpp

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <string>
#include <iostream>

#include "../arrayBuffers/arrayBuffer.hpp"
#include "../arrayBuffers/dynamicArrayBuffer.hpp"
#include "../rectangle.hpp"
#include "../shader.hpp"
#include "../texture.hpp"
#include "../utils.hpp"

#include "uiObject.hpp"

using namespace glm;

class UIRectangle: public UIObject {
public:
    UIRectangle();
    UIRectangle(Shader *shader, const RenderData *data);
    UIRectangle(Shader *shader, const RenderData *data, const vec2 *customUVs);
    
    void set(Shader *shader, const RenderData *data);
    void set(Shader *shader, const RenderData *data, const vec2 *customUVs);
    
    ~UIRectangle();
    
    void render();
    
    void setTexture(Texture texture);
    void setPosition(vec2 position);
    void setSize(vec2 size);
    void setPixelPosition(vec2 position);
    void setPixelSize(vec2 size);
    void setXTexOffset(float x);
    void setYTexOffset(float y);
    void setXTexOffset(float *x);
    void setYTexOffset(float *y);
    void setXTexMultiplier(float x);
    void setYTexMultiplier(float y);
    void setRotation(quat rotation);
    void addRotation(quat rotation);
    
    void changeUVs(const vec2 *UVs);
    
    Texture getTexture();
    vec2 getPosition();
    vec2 getSize();
    quat getRotation();
    mat4 getModelMat();
    
    float getXTexOffset();
    float getYTexOffset();
    
    Shader *getShaderPointer();
    
private:
    GLuint VAO;
    ArrayBuffer vertex;
    DynamicArrayBuffer texCoord;
    Texture tex;
    
    vec2 position;
    vec2 size;
    quat rotation;
    
    vec2 texOffset;
    vec2 texMultiplier;
    
    mat4 rotate, translate, scale, model;
    
    Shader *shader;
    const RenderData *data;
    float *xTexOffset, *yTexOffset;
    float rxTexOffset, ryTexOffset;
    bool noXTexOffset, noYTexOffset;
    
};

#endif /* ui_rectangle_hpp */
