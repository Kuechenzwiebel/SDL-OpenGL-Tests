//
//  item.cpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 20.08.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#include <stdio.h>

#include "item.hpp"

Item::Item(int itemID):
itemID(itemID), variation(0) {
    
}

bool Item::operator==(Item &other) {
    if(itemID == other.itemID && variation == other.variation) {
        return true;
    }
    else {
        return false;
    }
}



Slot::Slot(Shader *shader, RenderData *data):
storedType(Item(-1)), middlePos(glm::vec2(0.0f)), tex("resources/textures/items/t-" + std::to_string(storedType.itemID) + "-" + std::to_string(storedType.variation) + ".png"), rect(shader, data) {
    rect.setTexture(&tex);
    rect.setPixelSize(glm::vec2(32.0f));
}


void Slot::render() {
    rect.render();
}

glm::vec2 Slot::getRenderPosition() {
    return middlePos;
}

void Slot::setRenderPosition(glm::vec2 pos) {
    middlePos = pos;
    rect.setPixelPosition(middlePos);
}


Slot& Slot::operator+(Item &item) {
    if(items.size() == 0) {
        storedType = item;
        items.push_back(item);
        item.storedIn = this;
        
        tex = Texture("resources/textures/items/t-" + std::to_string(storedType.itemID) + "-" + std::to_string(storedType.variation) + ".png", TEXTURE_NO_MIP_MAP);
    }
    else {
        if(item == storedType) {
            items.push_back(item);
            item.storedIn = this;
        }
        else {
            throw new IllegalSlotAddition;
        }
    }
    return *this;
}

Slot& Slot::operator+=(Item &item) {
    if(items.size() == 0) {
        storedType = item;
        items.push_back(item);
        item.storedIn = this;
        
        tex = Texture("resources/textures/items/t-" + std::to_string(storedType.itemID) + "-" + std::to_string(storedType.variation) + ".png", TEXTURE_NO_MIP_MAP);
    }
    else {
        if(item == storedType) {
            items.push_back(item);
            item.storedIn = this;
        }
        else {
            throw new IllegalSlotAddition;
        }
    }
    return *this;
}
