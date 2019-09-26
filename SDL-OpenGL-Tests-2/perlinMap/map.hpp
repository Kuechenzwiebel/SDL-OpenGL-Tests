//
//  map.hpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 26.09.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#ifndef map_hpp
#define map_hpp

#include <stdio.h>
#include <vector>

#include "mapChunk.hpp"

class Map {
public:
    Map(unsigned int seed);
    
private:
    std::vector<std::unique_ptr<MapChunk>> chunks;
};

#endif /* map_hpp */
