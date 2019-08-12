//
//  cubemap.cpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 16.04.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#include "cubemap.hpp"

Cubemap::Cubemap(std::string* fileNames) {
    if(fileNames != NULL) {
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_CUBE_MAP, tex);
        
        for(int i = 0; i < 6; i++) {
            unsigned char *image = SOIL_load_image(fileNames[i].c_str(), &texWidth, &texHeight, 0, SOIL_LOAD_RGBA);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
            
            SOIL_free_image_data(image);
        }
        
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }
}

GLuint Cubemap::getData() {
    return tex;
}

void Cubemap::setTextureName(std::string name) {
    textureName = name;
}

std::string Cubemap::getTextureName() {
    return textureName;
}
