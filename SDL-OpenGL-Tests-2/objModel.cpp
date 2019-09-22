//
//  objModel.cpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 31.08.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#include "objModel.hpp"

ObjModel::ObjModel(std::string path, Shader *shader, const RenderData *data, bool *wireframe):
shader(shader), data(data), translate(1), rotate(1), scale(1), model(1), position(glm::vec3(0.0f)), size(glm::vec3(1.0f)), rotation(glm::vec4(0.0f)), wireframe(wireframe), realPositionSet(false) {
    realPosition = position;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    hg::File objFile(path);
    std::vector<std::string> fileLines = objFile.readFileLineByLine();
    std::stringstream lineStream;
    
    std::vector<glm::vec3> readVertices;
    std::vector<glm::vec2> readUVs;
    std::vector<glm::vec3> readNormals;
    
    int lastSize = 0;
    
    bool first = true;
    
    for(int i = 0; i < fileLines.size(); i++) {
        if(fileLines[i].substr(0, 2) == "o ") {
            if(!first) {
                ends[ends.size() - 1].second = int(vertices.size()) - lastSize;
            }
            ends.push_back(std::make_pair(vertices.size(), 0));
            lastSize = int(vertices.size());
            
            first = false;
            
            textures.emplace_back(Texture(hg::substr(path, 0, int(path.find_last_of("/"))) + "/" + hg::substr(fileLines[i], 2, int(fileLines[i].length())) + ".png"));
            std::cout << "Opening texture: " << hg::substr(path, 0, int(path.find_last_of("/"))) + "/" + hg::substr(fileLines[i], 2, int(fileLines[i].length())) + ".png" << std::endl;
        }
        
        else if(fileLines[i].substr(0, 2) == "v ") {
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
            
            lineStream
            >> v1 >> t1 >> n1
            >> v2 >> t2 >> n2
            >> v3 >> t3 >> n3;
            
            vertices.push_back(readVertices[v1 - 1]);
            vertices.push_back(readVertices[v2 - 1]);
            vertices.push_back(readVertices[v3 - 1]);
            
            uvs.push_back(readUVs[t1 - 1]);
            uvs.push_back(readUVs[t2 - 1]);
            uvs.push_back(readUVs[t3 - 1]);
            
            normals.push_back(readNormals[n1 - 1]);
            normals.push_back(readNormals[n2 - 1]);
            normals.push_back(readNormals[n3 - 1]);
        }
    }
    
    ends[ends.size() - 1].second = int(vertices.size()) - lastSize;
    
    vertex.setData(vertices.data(), sizeof(glm::vec3) * vertices.size(), 0);
    uv.setData(uvs.data(), sizeof(glm::vec2) * uvs.size(), 1);
    normal.setData(normals.data(), sizeof(glm::vec3) * normals.size(), 2);
    
    vertex.activate();
    uv.activate();
    normal.activate();
    
    printf("\n");
    
    glBindVertexArray(0);
}

ObjModel::~ObjModel() {
    glDeleteVertexArrays(1, &VAO);
}

void ObjModel::render() {
    vertex.activate();
    uv.activate();
    normal.activate();
    
    
    if(wireframe != nullptr)
        shader->sendInt(*wireframe, "wireframe");
    else
        shader->sendInt(0, "wireframe");
    
    for(int i = 0; i < ends.size(); i++) {
        glBindVertexArray(VAO);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[i].getTextureID());
        shader->sendInt(0, textures[i].getTextureName());
        
        shader->sendMat4(*data->projection, "projection");
        shader->sendMat4(data->viewMat, "view");
        shader->sendMat4(model, "model");
        
        glDrawArrays(GL_TRIANGLES, ends[i].first, ends[i].second);
        
        glBindVertexArray(0);
    }
}


void ObjModel::setRealPosition(glm::vec3 position) {
    this->realPosition = position;
    realPositionSet = true;
}

void ObjModel::setPosition(glm::vec3 position) {
    translate = glm::translate(glm::mat4(1), position);
    model = translate * rotate * scale;
    this->position = position;
    if(realPositionSet == false) {
        this->realPosition = position;
    }
}

void ObjModel::setSize(glm::vec3 size) {
    scale = glm::scale(glm::mat4(1), size);
    model = translate * rotate * scale;
    this->size = size;
}

void ObjModel::setRotation(glm::vec4 rotation) {
    rotate = glm::rotate(glm::mat4(1), rotation.w, rotation.xyz());
    model = translate * rotate * scale;
    this->rotation = rotation;
}

void ObjModel::setModelMat(glm::mat4 modelMat) {
    this->model = modelMat;
}


glm::vec3 ObjModel::getPosition() {
    return position;
}

glm::vec3 ObjModel::getRealPosition() {
    return realPosition;
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
