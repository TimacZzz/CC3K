#include "potion.h"

Potion::Potion(string type, int value, bool canWalk, bool canPick, string name, Tile tile): 
    Item{type, value, canWalk, canPick, tile}, name{name}{}

Potion::~Potion() {};

// Check whether the potion has been learnt or not
bool Potion::isLearned(){
    if (name == "RH"){
        return rh_learned;
    }
    else if (name == "PH"){
        return ph_learned;
    }
    else if (name == "WA"){
        return wa_learned;
    }
    else if (name == "WD"){
        return wd_learned;
    }
    else if (name == "BA"){
        return ba_learned;
    }
    else{
        return bd_learned;
    }
}

string Potion::getName(){
    return name;
}

int Potion::getPrice(){
    return value;
}

// Change the value of static fields when the potion is recognized after consuming it.
void Potion::learnPotion(string name){
    if (name == "RH"){
        rh_learned = true;
    }
    else if (name == "PH"){
        ph_learned = true;
    }
    else if (name == "WA"){
        wa_learned = true;
    }
    else if (name == "WD"){
        wd_learned = true;
    }
    else if (name == "BA"){
        ba_learned = true;
    }
    else{
        bd_learned = true;
    }
}

