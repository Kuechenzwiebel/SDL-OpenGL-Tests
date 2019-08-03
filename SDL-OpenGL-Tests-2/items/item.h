//
//  item.h
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 11.06.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#ifndef item_h
#define item_h

#include <glm/glm.hpp>
#include <vector>

#include "../texture.hpp"

struct Item;
struct Slot;
struct IllegalSlotAddition;

struct IllegalSlotAddition {
    
};

struct Item {
    Item(unsigned int itemID):
    itemID(itemID) {
        
    }
    
    Item():
    itemID(0) {
        
    }
    
    unsigned int itemID;
    unsigned int variation;
    
    Slot *storedIn;
    
    bool operator==(Item &other) {
        if(itemID == other.itemID && variation == other.variation) {
            return true;
        }
        else {
            return false;
        }
    }
};

struct Slot {
    std::vector<Item> items;
    Item storedType;
    
    glm::vec2 renderMiddlePos;
    
    Slot& operator+(Item &item) {
        if(items.size() == 0) {
            storedType = item;
            items.push_back(item);
            item.storedIn = this;
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
    
    Slot& operator+=(Item &item) {
        if(items.size() == 0) {
            storedType = item;
            items.push_back(item);
            item.storedIn = this;
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
};

#endif /* item_h */
