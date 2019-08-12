//
//  texture.cpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 07.03.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#include "texture.hpp"

Texture::Texture(std::string fileName):
textureName("tex") {
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    
    unsigned char *image = SOIL_load_image(fileName.c_str(), &texWidth, &texHeight, 0, SOIL_LOAD_RGBA);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
}

GLuint Texture::getData() {
    return tex;
}

void Texture::setTextureName(std::string name) {
    textureName = name;
}

std::string Texture::getTextureName() {
    return textureName;
}

glm::vec2 Texture::getTextureSize() {
    return glm::vec2(texWidth, texHeight);
}
