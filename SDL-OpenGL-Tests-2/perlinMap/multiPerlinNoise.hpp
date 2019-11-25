//
//  multiPerlinNoise.hpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 23.11.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#ifndef multiPerlinNoise_hpp
#define multiPerlinNoise_hpp

#include <stdio.h>
#include <vector>

#include "perlinNoise.hpp"

class MultiPerlinNoise: public PerlinNoise {
public:
    MultiPerlinNoise();
    
    std::vector<PerlinNoise*> perlinNoises;
    
    float octaveNoise(float x, float y);
};

#endif /* multiPerlinNoise_hpp */
