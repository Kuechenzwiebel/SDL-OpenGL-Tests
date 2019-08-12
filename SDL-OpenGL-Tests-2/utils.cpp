//
//  utils.cpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 15.03.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#include "utils.hpp"

void printVec2(glm::vec2 data) {
    printf("X = %f\tY = %f\n", data.x, data.y);
}

void printVec3(glm::vec3 data) {
    printf("X = %f\tY = %f\tZ = %f\n", data.x, data.y, data.z);
}
void printVec4(glm::vec4 data) {
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
void printArray(glm::vec3 *array, int arraySize) {
    for(int i = 0; i < arraySize; i++) {
        std::cout << i << ". = ";
        printVec3(array[i]);
    }
}

void printfMat4x4(glm::mat4 m) {
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

glm::vec2 pixelPosToUIRect(glm::vec2 pixel, glm::vec2 rectSize) {
    return glm::vec2(pixel * (glm::vec2(1.0f) / (rectSize * glm::vec2(500.0f))));
}

glm::vec2 pixelSizeToUIRect(glm::vec2 size) {
    return glm::vec2(size / glm::vec2(1000.0f));
}

glm::vec2 UIRectToPixelPos(glm::vec2 rectSize) {
    return glm::vec2(rectSize * glm::vec2(1.0f / 500.0f));
}

glm::vec2 UIRectToPixelSize(glm::vec2 size) {
    return glm::vec2(size * glm::vec2(1000.0f));
}
