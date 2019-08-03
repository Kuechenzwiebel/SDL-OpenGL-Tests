//
//  file.cpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 07.03.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#include "file.hpp"

File::File(string fileName):
fileName(fileName) {
    file = ifstream(fileName);
    
    if(!file) {
        printf("\033[0;31m");
        printf("Cant open the file: %s\n", fileName.c_str());
        printf("\033[0m");
    }
}

File::~File() {
    file.close();
}

vector <string> File::readFileLineByLine() {
    vector <string> vec;
    
    string str;
    while (getline(file, str)) {
        if(str.size() > 0) {
            vec.push_back(str);
        }
    }
    return vec;
}

string File::readFile() {
    std::string str;
    
    try {
        std::stringstream contentStream;
        
        contentStream << file.rdbuf();
        str = contentStream.str();
    }
    catch (std::ifstream::failure e) {
        printf("File could not be read!\n");
    }
    
    return str;
}
