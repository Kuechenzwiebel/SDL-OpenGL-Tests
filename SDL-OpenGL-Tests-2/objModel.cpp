//
//  objModel.cpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 31.08.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#include "objModel.hpp"

ObjModel::ObjModel(std::string path, Shader *shader, const RenderData *data):
shader(shader), data(data), translate(1), rotate(1), scale(1), model(1), position(glm::vec3(0.0f)), size(glm::vec3(1.0f)) {
    
    hg::File objFile(path);
    std::vector<std::string> fileLines = objFile.readFileLineByLine();
    std::stringstream lineStream;
    
    std::vector<glm::vec3> readVertices;
    std::vector<glm::vec2> readUVs;
    std::vector<glm::vec3> readNormals;
    ObjModelComponent tmpComp;
    Texture tmpTex("");
    
    int componentIndex = 0;
    bool first = true;
    
    textures[0] = Texture("resources/models/cube.png");
    textures[1] = Texture("resources/models/cube2.png");
    textures[2] = Texture("resources/models/cube3.png");
    
    for(int i = 0; i < fileLines.size(); i++) {
        if(fileLines[i].substr(0, 2) == "o ") {
            if(!first) {
                tmpComp.tex = tmpTex;
                components.emplace_back(tmpComp);
                componentIndex++;
            }
            
            first = false;
            
            tmpTex = Texture(hg::substr(path, 0, int(path.find_last_of("/"))) + "/" + hg::substr(fileLines[i], 2, int(fileLines[i].length())) + ".png");
            std::cout << "Opening Texture: "<< hg::substr(path, 0, int(path.find_last_of("/"))) + "/" + hg::substr(fileLines[i], 2, int(fileLines[i].length())) + ".png" << std::endl;
        }
        
        if(fileLines[i].substr(0, 2) == "v ") {
            float x, y, z;
            lineStream = std::stringstream(hg::substr(fileLines[i], 2, int(fileLines[i].length())));
            lineStream >> x >> y >> z;
            readVertices.push_back(glm::vec3(x, y, z));
        }
        
        else if(fileLines[i].substr(0, 2) == "vt") {
            float u, v;
            lineStream = std::stringstream(hg::substr(fileLines[i], 2, int(fileLines[i].length())));
            lineStream >> u >> v;
            readUVs.push_back(glm::vec2(u, v));
        }
        
        else if(fileLines[i].substr(0, 2) == "vn") {
            float x, y, z;
            lineStream = std::stringstream(hg::substr(fileLines[i], 2, int(fileLines[i].length())));
            lineStream >> x >> y >> z;
            readNormals.push_back(glm::vec3(x, y, z));
        }
        
        else if(fileLines[i].substr(0, 2) == "f ") {
            std::replace(fileLines[i].begin(), fileLines[i].end(), '/', ' ');
            lineStream = std::stringstream(hg::substr(fileLines[i], 2, int(fileLines[i].length())));
            
            int v1, v2, v3;
            int t1, t2, t3;
            int n1, n2, n3;
            
            lineStream >> v1 >> t1 >> n1
            >> v2 >> t2 >> n2
            >> v3 >> t3 >> n3;
            
            tmpComp.vertices.push_back(readVertices[v1 - 1]);
            tmpComp.vertices.push_back(readVertices[v2 - 1]);
            tmpComp.vertices.push_back(readVertices[v3 - 1]);
            
            tmpComp.uvs.push_back(readUVs[t1 - 1]);
            tmpComp.uvs.push_back(readUVs[t2 - 1]);
            tmpComp.uvs.push_back(readUVs[t3 - 1]);
            
            tmpComp.normals.push_back(readNormals[n1 - 1]);
            tmpComp.normals.push_back(readNormals[n2 - 1]);
            tmpComp.normals.push_back(readNormals[n3 - 1]);
        }
    }
    
    tmpComp.tex = tmpTex;
    components.emplace_back(tmpComp);
    
    /*
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);*/
    
    for(int i = 0; i < components.size(); i++) {
        glGenVertexArrays(1, &components[i].VAO);
        glBindVertexArray(components[i].VAO);
        
        components[i].vertex.setData(components[i].vertices.data(), sizeof(glm::vec3) * components[i].vertices.size(), 0);
        components[i].uv.setData(components[i].uvs.data(), sizeof(glm::vec2) * components[i].uvs.size(), 1);
        components[i].normal.setData(components[i].normals.data(), sizeof(glm::vec3) * components[i].normals.size(), 2);
        
        components[i].vertex.activate();
        components[i].uv.activate();
        components[i].normal.activate();
        
        glBindVertexArray(0);
    }
    
//    glBindVertexArray(0);
}

ObjModel::~ObjModel() {
    for(int i = 0; i < components.size(); i++) {
        glDeleteVertexArrays(1, &components[i].VAO);
    }
}

void ObjModel::render() {
    for(int i = 0; i < components.size(); i++) {
        components[i].vertex.activate();
        components[i].uv.activate();
        components[i].normal.activate();
         
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, components[i].tex.getTextureID());
        shader->sendInt(0, components[i].tex.getTextureName());
        
        glBindVertexArray(components[i].VAO);
        shader->sendMat4(*data->projection, "projection");
        shader->sendMat4(data->viewMat, "view");
        shader->sendMat4(model, "model");
        
        glDrawArrays(GL_TRIANGLES, 0, int(components[i].vertices.size()));
        glBindVertexArray(0);
    }
}


void ObjModel::setPosition(glm::vec3 position) {
    translate = glm::translate(glm::mat4(1), position);
    model = translate * rotate * scale;
    this->position = position;
}

void ObjModel::setSize(glm::vec3 size) {
    rotate = glm::rotate(glm::mat4(1), rotation.w, rotation.xyz());
    model = translate * rotate * scale;
    this->size = size;
}

void ObjModel::setRotation(glm::vec4 rotation) {
    scale = glm::scale(glm::mat4(1), size);
    model = translate * rotate * scale;
    this->rotation = rotation;
}

glm::vec3 ObjModel::getPosition() {
    return position;
}

glm::vec4 ObjModel::getRotation() {
    return rotation;
}

glm::vec3 ObjModel::getSize() {
    return size;
}

Shader* ObjModel::getShaderPointer() {
    return shader;
}
