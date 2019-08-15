//
//  rectangle.hpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 30.03.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#ifndef ui_rectangle_hpp
#define ui_rectangle_hpp

#define GLM_SWIZZLE

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <iostream>

#include "../arrayBuffers/arrayBuffer.hpp"
#include "../arrayBuffers/dynamicArrayBuffer.hpp"
#include "../rectangle.hpp"
#include "../shader.hpp"
#include "../texture.hpp"
#include "../utils.hpp"

#include "uiObject.hpp"

class UIRectangle: public UIObject {
public:
    UIRectangle();
    UIRectangle(Shader *shader, const RenderData *data);
    UIRectangle(Shader *shader, const RenderData *data, const glm::vec2 *customUVs);
    
    void set(Shader *shader, const RenderData *data);
    void set(Shader *shader, const RenderData *data, const glm::vec2 *customUVs);
    
    ~UIRectangle();
    
    void render();
    
    void setTexture(Texture texture);
    void setPosition(glm::vec2 position);
    void setSize(glm::vec2 size);
    void setPixelPosition(glm::vec2 position);
    void setPixelSize(glm::vec2 size);
    void setXTexOffset(float x);
    void setYTexOffset(float y);
    void setXTexOffset(float *x);
    void setYTexOffset(float *y);
    void setXTexMultiplier(float x);
    void setYTexMultiplier(float y);
    
    void changeUVs(const glm::vec2 *UVs);
    
    Texture getTexture();
    glm::vec2 getPosition();
    glm::vec2 getSize();
    glm::mat4 getModelMat();
    
    float getXTexOffset();
    float getYTexOffset();
    
    Shader *getShaderPointer();
    
private:
    GLuint VAO;
    ArrayBuffer vertex;
    DynamicArrayBuffer texCoord;
    Texture tex;
    
    glm::vec2 position;
    glm::vec2 size;
    
    glm::vec2 texOffset;
    glm::vec2 texMultiplier;
    
    glm::mat4 translate, scale, model;
    
    Shader *shader;
    const RenderData *data;
    float *xTexOffset, *yTexOffset;
    float rxTexOffset, ryTexOffset;
    bool noXTexOffset, noYTexOffset;
    
};

#endif /* ui_rectangle_hpp */
