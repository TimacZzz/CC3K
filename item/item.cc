#include "item.h"

// Constructor
Item::Item(string type, int value, bool canWalk, bool canPick, Tile tile): type{type},
    value{value}, canWalk{canWalk}, canPick{canPick}, currentTile{tile}{}

// Some useful accessors and mutators
void Item::setTile(Tile destination){
    currentTile = destination;
}

void Item::setCanPick(bool value){
    canPick = value;
}

void Item::setCanWalk(bool value){
    canWalk = value;
}

Tile Item::getTile(){
    return currentTile;
}

bool Item::getCanPick(){
    return canPick;
}

bool Item::getCanWalk(){
    return canWalk;
}

string Item::getType(){
    return type;
}
