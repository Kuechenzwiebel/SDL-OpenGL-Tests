//
//  uiText.hpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 18.04.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#ifndef uiText_hpp
#define uiText_hpp

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <iostream>
#include <vector>

#include "uiRectangle.hpp"
#include "../shader.hpp"
#include "../utils.hpp"
#include "../texture.hpp"

#include "uiObject.hpp"

static float charWidth = 42.0f;
static float charHeight = 68.0f;

class UIText: public UIObject {
public:
    UIText(std::string s, Shader *shader, const RenderData *data);
    ~UIText();
    
    void setText(std::string s);
    
    void render();
    
    void setPosition(glm::vec2 position);
    void setSize(glm::vec2 size);
    void setPixelPosition(glm::vec2 position);
    void setPixelSize(glm::vec2 size);
    
    glm::vec2 getPosition();
    glm::vec2 getSize();
    
    Shader *getShaderPointer();
    
private:
    Texture charSet;
    Shader *shader;
    const RenderData *data;
    
    glm::vec2 position;
    glm::vec2 size;
    
    std::string s;
    std::vector<UIRectangle> rects;
    std::vector<glm::vec2> originalPositions;
};


#endif /* uiText_hpp */
