//
//  uiObject.hpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 03.08.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#ifndef uiObject_hpp
#define uiObject_hpp

#include <stdio.h>

#include "../shader.hpp"

class UIObject {
public:
    UIObject();
    
    virtual void render();
    virtual Shader *getShaderPointer();
};

#endif /* uiObject_hpp */
