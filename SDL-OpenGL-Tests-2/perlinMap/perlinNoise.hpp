//
//  perlinNoise.hpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 26.09.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#ifndef perlinNoise_hpp
#define perlinNoise_hpp

#include <stdio.h>

#include "sivPerlinNoise.hpp"

class PerlinNoise {
public:
    PerlinNoise(unsigned int seed);
    
    float octaveNoise(float x, float y);
    
    float frequency;
    float multiplier;
    int octaves;
    unsigned int seed;
    float offset;
    
private:
    siv::PerlinNoise perlinNoise;
};

#endif /* perlinNoise_hpp */
