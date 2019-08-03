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

using namespace std;


class Texture {
public:
    Texture(string fileName);
    
    GLuint getData();
    
    void setTextureName(string name);
    string getTextureName();
    
    glm::vec2 getTextureSize();
    
private:
    GLuint tex;    
    int texWidth, texHeight;
    string textureName;
};

#endif /* texture_hpp */
