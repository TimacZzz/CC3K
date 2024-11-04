#include "gold.h"

// Constructor
Gold::Gold(string name, int value, bool canWalk, bool canPick, Tile tile): 
    Item{"Gold", value, canWalk, canPick, tile},name{name}{
}

// Some useful accessors and mutators
int Gold::getValue(){
    return value;
}

void Gold::setValue(int value){
    this->value = value;
}

string Gold::getName(){
    return name;
}

void Gold::setName(string name){
    this->name = name;
}
