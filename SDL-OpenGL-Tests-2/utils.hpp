//
//  utils.hpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 15.03.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#ifndef utils_hpp
#define utils_hpp

#include <stdio.h>
#include <iostream>
#include <glm/glm.hpp>

#define PRINTF_RED "\033[0;31m"
#define PRINTF_BLUE "\033[0;34m"
#define PRINTF_GREEN "\033[0;32m"
#define PRINTF_DEFAULT "\033[0m"

void printVec2(glm::vec2 data);
void printVec3(glm::vec3 data);
void printVec4(glm::vec4 data);

template <typename T>
void printArray(T *array, int arraySize);
template <>
void printArray(glm::vec3 *array, int arraySize);

void printfMat4x4(glm::mat4 m);

struct RenderData {
    glm::mat4 viewMat;
    glm::mat4 *projection;
    int *windowWidth;
    int *windowHeight;
};

void swapBool(bool *b);

glm::vec2 pixelPosToUIRect(glm::vec2 pixel, glm::vec2 rectSize);
glm::vec2 pixelSizeToUIRect(glm::vec2 size);
glm::vec2 UIRectToPixelPos(glm::vec2 rectSize);
glm::vec2 UIRectToPixelSize(glm::vec2 size);

#endif /* utils_hpp */
