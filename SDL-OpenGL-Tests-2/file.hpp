//
//  file.hpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 07.03.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#ifndef file_hpp
#define file_hpp

#include <iostream>
#include <stdio.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

class File {
public:
    File(string fileName);
    
    ~File();
    
    vector <string> readFileLineByLine();
    string readFile();
    
private:
    string fileName;
    ifstream file;
};

#endif /* file_hpp */
