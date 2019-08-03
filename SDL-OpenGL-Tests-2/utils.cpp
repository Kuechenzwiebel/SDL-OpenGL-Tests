//
//  utils.cpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 15.03.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#include "utils.hpp"

void printVec2(vec2 data) {
    printf("X = %f\tY = %f\n", data.x, data.y);
}

void printVec3(vec3 data) {
    printf("X = %f\tY = %f\tZ = %f\n", data.x, data.y, data.z);
}
void printVec4(vec4 data) {
    printf("X = %f\tY = %f\tZ = %f\tW = %f\n", data.x, data.y, data.z, data.w);
}

template <typename T>
void printArray(T *array, int arraySize) {
    for(int i = 0; i < arraySize; i++) {
        std::cout << i << ". = " << array[i] << " ";
    }
    std::cout << std::endl;
}

template <>
void printArray(vec3 *array, int arraySize) {
    for(int i = 0; i < arraySize; i++) {
        std::cout << i << ". = ";
        printVec3(array[i]);
    }
}

void printfMat4x4(mat4 m) {
    printf("--------------------------------------------------\n"
           "%f\t%f\t%f\t%f\t\n"
           "%f\t%f\t%f\t%f\t\n"
           "%f\t%f\t%f\t%f\t\n"
           "%f\t%f\t%f\t%f\t\n"
           "--------------------------------------------------\n",
           m[0][0], m[1][0], m[2][0], m[3][0],
           m[0][1], m[1][1], m[2][1], m[3][1],
           m[0][2], m[1][2], m[2][2], m[3][2],
           m[0][3], m[1][3], m[2][3], m[3][3]);
}

void swapBool(bool *b) {
    *b = !(*b);
}

vec2 pixelPosToUIRect(vec2 pixel, vec2 rectSize) {
    return vec2(pixel * (vec2(1.0f) / (rectSize * vec2(500.0f))));
}

vec2 pixelSizeToUIRect(vec2 size) {
    return vec2(size / vec2(1000.0f));
}

vec2 UIRectToPixelPos(vec2 rectSize) {
    return vec2(rectSize * vec2(1.0f / 500.0f));
}

vec2 UIRectToPixelSize(vec2 size) {
    return vec2(size * vec2(1000.0f));
}


vec3 btVec3ToGLM(btVector3 vec) {
    return vec3(vec.x(), vec.y(), vec.z());
}

btVector3 GLMVec3ToBt(vec3 vec) {
    return btVector3(vec.x, vec.y, vec.z);
}

