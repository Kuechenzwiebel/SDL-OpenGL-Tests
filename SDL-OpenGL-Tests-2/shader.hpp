//
//  shader.hpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 08.03.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#ifndef shader_hpp
#define shader_hpp

#include <stdio.h>
#include <string>

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "utils.hpp"

class Shader {
public:
    Shader(std::string vertexCode, std::string fragmentCode);
    
    void use();
    
    void sendVec2(glm::vec2 data, std::string name);
    void sendVec3(glm::vec3 data, std::string name);
    void sendVec4(glm::vec4 data, std::string name);
    void sendMat4(glm::mat4 data, std::string name);
    void sendInt(int data, std::string name);
    void sendFloat(float data, std::string name);
    
    GLint getLoc(std::string name);
    
    GLuint program;
    
private:
    void compileShader(const char* vertexCode, const char* fragmentCode);
    
    std::string _vertexCode, _fragmentCode;
};

#endif /* shader_hpp */
