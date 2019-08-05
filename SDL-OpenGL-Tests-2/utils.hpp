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

using namespace glm;

void printVec2(vec2 data);
void printVec3(vec3 data);
void printVec4(vec4 data);

template <typename T>
void printArray(T *array, int arraySize);
template <>
void printArray(vec3 *array, int arraySize);

void printfMat4x4(mat4 m);

struct RenderData {
    mat4 viewMat;
    mat4 *projection;
    int *windowWidth;
    int *windowHeight;
};

void swapBool(bool *b);

vec2 pixelPosToUIRect(vec2 pixel, vec2 rectSize);
vec2 pixelSizeToUIRect(vec2 size);
vec2 UIRectToPixelPos(vec2 rectSize);
vec2 UIRectToPixelSize(vec2 size);

#endif /* utils_hpp */
