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
    Item(int itemID);
    
    int itemID;
    unsigned int variation;
    
    Slot *storedIn;
    
    bool operator==(Item &other);
};

class Slot {
public:
    Slot(Shader *shader, RenderData *data);
    
    std::vector<Item> items;
    Item storedType;
    
    void render();
    
    glm::vec2 getRenderPosition();
    void setRenderPosition(glm::vec2 pos);
    
    Slot& operator+(Item &item);
    Slot& operator+=(Item &item);
    
private:
    UIRectangle rect;
    Texture tex;   
    
    glm::vec2 middlePos;
};

#endif /* item_hpp */
