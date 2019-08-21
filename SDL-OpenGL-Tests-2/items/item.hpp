//
//  item.h
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 11.06.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#ifndef item_hpp
#define item_hpp

#include <glm/glm.hpp>
#include <vector>
#include <string>

#include "../texture.hpp"
#include "../shader.hpp"
#include "../utils.hpp"
#include "../ui/uiRectangle.hpp"

struct Item;
struct Slot;

struct IllegalSlotAddition {
    
};

struct Item {
    Item(unsigned int itemID);
    
    unsigned int itemID;
    unsigned int variation;
    
    Slot *storedIn;
    
    bool operator==(Item &other);
};

struct Slot {
    Slot();
    
    std::vector<Item> items;
    Item storedType;
    
    glm::vec2 renderMiddlePos;
    
    Slot& operator+(Item &item);
    Slot& operator+=(Item &item);
};

void renderSlot(Slot *slot, Shader *shader, RenderData *data);

#endif /* item_hpp */
