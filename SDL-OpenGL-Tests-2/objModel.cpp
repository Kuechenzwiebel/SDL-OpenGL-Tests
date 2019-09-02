//
//  objModel.cpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 31.08.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#include "objModel.hpp"

ObjModelComponent::ObjModelComponent():
tex("") {
    
}

ObjModel::ObjModel(std::string path, Shader *shader, const RenderData *data):
shader(shader), data(data), translate(1), rotate(1), scale(1), model(1), position(glm::vec3(0.0f)), size(glm::vec3(1.0f)) {
    
    
    hg::File objFile(path);
    std::vector<std::string> objLines = objFile.readFileLineByLine();
    std::stringstream lineStream;
    
    std::vector<glm::vec3> readVertices;
    std::vector<glm::vec2> readUVs;
    std::vector<glm::vec3> readNormals;
    
    std::vector<glm::vec3> tmpVertices;
    std::vector<glm::vec2> tmpUVs;
    std::vector<glm::vec3> tmpNormals;
    
    Texture tmpTexture("");
    
    int componentIndex = 0;
    
    numObjects = 0;
    
    for(int i = 0; i < objLines.size(); i++) {
        if(objLines[i][0] == 'o') {
            numObjects ++;
        }
    }
    
    VAOs.resize(numObjects);
    vertex.resize(numObjects);
    uv.resize(numObjects);
    normal.resize(numObjects);
    
    bool first = true;
    
    for(int i = 0; i < objLines.size(); i++) {
        if(objLines[i].substr(0, 2) == "o ") {
            if(!first) {
                vertices.push_back(tmpVertices);
                uvs.push_back(tmpUVs);
                normals.push_back(tmpNormals);
                
                tmpVertices.clear();
                tmpUVs.clear();
                tmpNormals.clear();
                
                readVertices.clear();
                readUVs.clear();
                readNormals.clear();
                
                componentIndex++;
            }
            
            first = false;
            
//            tmpTexture = Texture(hg::substr(path, 0, int(path.find_last_of("/"))) + "/" + hg::substr(objLines[i], 2, int(objLines[i].length())) + ".png");
            
            std::cout << "Opening Texture: "<< hg::substr(path, 0, int(path.find_last_of("/"))) + "/" + hg::substr(objLines[i], 2, int(objLines[i].length())) + ".png" << std::endl;
        }
        
        else if(objLines[i].substr(0, 2) == "v ") {
            float x, y, z;
            lineStream = std::stringstream(hg::substr(objLines[i], 2, int(objLines[i].length())));
            lineStream >> x >> y >> z;
            readVertices.push_back(glm::vec3(x, y, z));
        }
        
        else if(objLines[i].substr(0, 2) == "vt") {
            float u, v;
            lineStream = std::stringstream(hg::substr(objLines[i], 2, int(objLines[i].length())));
            lineStream >> u >> v;
            readUVs.push_back(glm::vec2(u, v));
        }
        
        else if(objLines[i].substr(0, 2) == "vn") {
            float x, y, z;
            lineStream = std::stringstream(hg::substr(objLines[i], 2, int(objLines[i].length())));
            lineStream >> x >> y >> z;
            readNormals.push_back(glm::vec3(x, y, z));
        }
        
        else if(objLines[i].substr(0, 2) == "f ") {
            std::replace(objLines[i].begin(), objLines[i].end(), '/', ' ');
            lineStream = std::stringstream(hg::substr(objLines[i], 2, int(objLines[i].length())));
            
            int v1, v2, v3;
            int t1, t2, t3;
            int n1, n2, n3;
            
            lineStream >> v1 >> t1 >> n1
                       >> v2 >> t2 >> n2
                       >> v3 >> t3 >> n3;
            
            tmpVertices.push_back(readVertices[v1 - 1]);
            tmpVertices.push_back(readVertices[v2 - 1]);
            tmpVertices.push_back(readVertices[v3 - 1]);
            
            tmpUVs.push_back(readUVs[t1 - 1]);
            tmpUVs.push_back(readUVs[t2 - 1]);
            tmpUVs.push_back(readUVs[t3 - 1]);
            
            tmpNormals.push_back(readNormals[n1 - 1]);
            tmpNormals.push_back(readNormals[n2 - 1]);
            tmpNormals.push_back(readNormals[n3 - 1]);
        }
    }
    
    for(int i = 0; i < numObjects; i++) {
        glGenVertexArrays(1, &VAOs[i]);
        glBindVertexArray(VAOs[i]);
        
        vertex[i].setData(vertices[i].data(), sizeof(glm::vec3) * vertices[i].size(), 0);
        uv[i].setData(uvs[i].data(), sizeof(glm::vec2) * uvs[i].size(), 1);
        normal[i].setData(normals[i].data(), sizeof(glm::vec3) * normals[i].size(), 2);
        
        vertex[i].activate();
        uv[i].activate();
        normal[i].activate();
        
        glBindVertexArray(0);
    }
    /*
    glGenVertexArrays(1, &VAOs[componentIndex]);
    glBindVertexArray(VAOs[componentIndex]);
    
    components[componentIndex].vertex.setData(components[componentIndex].vertices.data(), sizeof(glm::vec3) * components[componentIndex].vertices.size(), 0);
    components[componentIndex].uv.setData(components[componentIndex].uvs.data(), sizeof(glm::vec2) * components[componentIndex].uvs.size(), 1);
    components[componentIndex].normal.setData(components[componentIndex].normals.data(), sizeof(glm::vec3) * components[componentIndex].normals.size(), 2);

    components[componentIndex].vertex.activate();
    components[componentIndex].uv.activate();
    components[componentIndex].normal.activate();
    
    glBindVertexArray(0);*/
}

ObjModel::~ObjModel() {
    for(int i = 0; i < VAOs.size(); i++) {
        glDeleteVertexArrays(1, &VAOs[i]);
    }
}

void ObjModel::render() {/*
    for(int i = 0; i < numObjects; i++) {
        vertex[i].activate();
        uv[i].activate();
        normal[i].activate();
        /*
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[i].getData());
        shader->sendInt(0, textures[i].getTextureName());
        
        glBindVertexArray(VAOs[i]);
        shader->sendMat4(*data->projection, "projection");
        shader->sendMat4(data->viewMat, "view");
        shader->sendMat4(model, "model");
        
        glDrawArrays(GL_TRIANGLES, 0, int(vertices[i].size()));
        glBindVertexArray(0);
    }*/
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
