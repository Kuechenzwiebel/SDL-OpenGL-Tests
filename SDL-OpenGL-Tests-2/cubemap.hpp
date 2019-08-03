//
//  cubemap.hpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 16.04.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#ifndef cubemap_hpp
#define cubemap_hpp

#include <GL/glew.h>
#include <stdio.h>
#include <string>
#include <SOIL2/SOIL2.h>
#include <iostream>

using namespace std;

class Cubemap {
public:
    Cubemap(string* fileNames);
    
    GLuint getData();
    
    void setTextureName(string name);
    string getTextureName();
    
private:
    GLuint tex;
    int texWidth, texHeight;
    string textureName;
};

#endif /* cubemap_hpp */
