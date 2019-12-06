//
//  objModel.hpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 31.08.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#ifndef objModel_hpp
#define objModel_hpp

#define GLM_FORCE_SWIZZLE

#include <stdio.h>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <HG_List/HG_List.h>

#include "utils.hpp"
#include "shader.hpp"
#include "object.hpp"
#include "arrayBuffers/arrayBuffer.hpp"
#include "texture.hpp"

class ObjModel: public Object {
public:
    ObjModel(std::string path, Shader *shader, const RenderData *data, bool *wireframe = nullptr, bool autoLoadTextures = true);
    ~ObjModel();
    
    void render();
    
    void setPosition(glm::vec3 position);
    void setRealPosition(glm::vec3 position);
    void setSize(glm::vec3 size);
    void setRotation(glm::vec4 rotation);
    void setModelMat(glm::mat4 modelMat);
    
    void setTextures(std::vector<Texture> *textures);
    
    glm::vec3 getPosition();
    glm::vec4 getRotation();
    glm::vec3 getSize();
    glm::vec3 getRealPosition();
    
    Shader *getShaderPointer();
    
private:
    ArrayBuffer vertex, uv, normal;
    GLuint VAO;
    
    Shader *shader;
    const RenderData *data;
    
    bool *wireframe;
    
    
    glm::vec3 realPosition;
    bool realPositionSet;
    glm::vec3 position;
    glm::vec4 rotation;
    glm::vec3 size;
    
    glm::mat4 translate, rotate, scale, model;
    
    std::vector<Texture> textures;
    
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    
    std::vector<std::pair<int, int>> ends;
};

#endif /* objModel_hpp */
