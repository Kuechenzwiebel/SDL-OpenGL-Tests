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

using namespace std;

class Shader {
public:
    Shader(string vertexCode, string fragmentCode);
    
    void use();
    
    void sendVec2(glm::vec2 data, string name);
    void sendVec3(glm::vec3 data, string name);
    void sendVec4(glm::vec4 data, string name);
    void sendMat4(glm::mat4 data, string name);
    void sendInt(int data, string name);
    void sendFloat(float data, string name);
    
    GLint getLoc(string name);
    
    GLuint program;
    
private:
    void compileShader(const char* vertexCode, const char* fragmentCode);
    
    string _vertexCode, _fragmentCode;
};

#endif /* shader_hpp */
