//
//  item.cpp
//  SDL-OpenGL-Tests-2
//
//  Created by Tobias Pflüger on 20.08.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#include <stdio.h>

#include "item.hpp"

Item::Item(unsigned int itemID):
itemID(itemID) {
    
}

bool Item::operator==(Item &other) {
    if(itemID == other.itemID && variation == other.variation) {
        return true;
    }
    else {
        return false;
    }
}


Slot::Slot():
storedType(Item(0)) {
    
}

Slot& Slot::operator+(Item &item) {
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

Slot& Slot::operator+=(Item &item) {
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
