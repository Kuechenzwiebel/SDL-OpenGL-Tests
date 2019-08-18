//
//  texture.hpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 07.03.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#ifndef texture_hpp
#define texture_hpp

#define GLEW_STATIC

#include <GL/glew.h>
#include <stdio.h>
#include <string>
#include <SOIL2/SOIL2.h>
#include <glm/glm.hpp>

enum TextureType {
    TEXTURE_GENERATE_MIPMAP,
    TEXTURE_NO_MIP_MAP
};

class Texture {
public:
    Texture(std::string fileName, TextureType type = TEXTURE_GENERATE_MIPMAP);
    
    GLuint getData();
    
    void setTextureName(std::string name);
    std::string getTextureName();
    
    glm::vec2 getTextureSize();
    
private:
    GLuint tex;    
    int texWidth, texHeight;
    std::string textureName;
};

#endif /* texture_hpp */
