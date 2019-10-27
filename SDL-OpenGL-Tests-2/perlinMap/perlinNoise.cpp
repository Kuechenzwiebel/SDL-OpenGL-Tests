//
//  perlinNoise.cpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 26.09.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#include "perlinNoise.hpp"

PerlinNoise::PerlinNoise(unsigned int seed):
perlinNoise(seed), frequency(2.0f), multiplier(1.0f), octaves(1), seed(seed), offset(0.0f) {

}
   
float PerlinNoise::octaveNoise(float x, float y) {
    return perlinNoise.octaveNoise(x / frequency, y / frequency, octaves) * multiplier + offset;
}
