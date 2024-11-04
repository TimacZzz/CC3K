#include "merchant.h"
#include "../../item/potion.h"
#include "../../item/rh.h"
#include "../../item/ba.h"
#include "../../item/bd.h"
#include <iostream>

Merchant::Merchant(Tile tile): Enemy{30, 30, 70, 70, 5, 5, 4, "Merchant", tile}{
    symbol = 'M';
    storage.push_back(new RH(Tile{-1, -1}));
    storage.push_back(new BA(Tile{-1, -1}));
    storage.push_back(new BD(Tile{-1, -1}));
    storage.push_back(new RH(Tile{-1, -1}));
    storage.push_back(new BA(Tile{-1, -1}));
    storage.push_back(new BD(Tile{-1, -1}));
}

// Print the Merchant's storage
void Merchant::printStorage(string &action_message){
    for (int i = 0; i < storage.size(); i++){
        if (i == storage.size() - 1){
            action_message += "Position " + std::to_string(i) + ": " + storage[i]->getName() + " Potion (" + std::to_string(storage[i]->getPrice()) + " gold) ";
            break;
        }
        action_message += "Position " + std::to_string(i) + ": " + storage[i]->getName() + " Potion (" + std::to_string(storage[i]->getPrice()) + " gold) \n";
    }
}

// Remove the sold item from the storage
void Merchant::sellItem(int position){
    storage.erase(storage.begin() + position);
}

Merchant::~Merchant(){
    for (int i = 0; i < storage.size(); i++){
        delete storage[i];
    }
}
