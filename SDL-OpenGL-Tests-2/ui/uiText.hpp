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
#include "../file.hpp"
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
    
    void setPosition(vec2 position);
    void setSize(vec2 size);
    void setPixelPosition(vec2 position);
    void setPixelSize(vec2 size);
    
    vec2 getPosition();
    vec2 getSize();
    
    Shader *getShaderPointer();
    
private:
    Texture charSet;
    Shader *shader;
    const RenderData *data;
    
    vec2 position;
    vec2 size;
    
    std::string s;
    std::vector<UIRectangle> rects;
    std::vector<vec2> originalPositions;
};


#endif /* uiText_hpp */
