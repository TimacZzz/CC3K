#include "character.h"
#include "../item/potion.h"

Character::Character(int maxHP, int curHP, int initATK, int curATK, int initDEF, int curDEF, int goldPossessed, string race, Tile tile):
    maxHP{maxHP}, curHP{curHP}, initATK{initATK}, curATK{curATK}, initDEF{initDEF}, curDEF{curDEF}, goldPossessed{goldPossessed},
    currentTile{tile}, race{race}{

}

Character::~Character(){}

int Character::getDef(){
    curDEF = initDEF;
    for (auto ob : observers) ob->notify(this);
    return curDEF;
}

int Character::getAtk(){
    curATK = initATK;
    for (auto ob : observers) ob->notify(this);
    return curATK;
}

int Character::getHp(){
    return curHP;
}

void Character::mutateHp(int value){
    curHP += value;
    //Cannot Drop 0
    if (curHP < 0){
        curHP = 0;
    }
    //Cannot Exceed MaxHP unless it's vampire
    if (!(race == "Vampire") && curHP > maxHP){
        curHP = maxHP;
    }
}

void Character::mutateAtk(int value){
    curATK += value;
}

void Character::mutateDef(int value){
    curDEF += value;
}

string Character::getRace(){
    return race;
}

void Character::mutateMaxHp(int value){
    maxHP += value;
}

Tile Character::getTile(){
    return currentTile;
}

int Character::getGold(){
    return goldPossessed;
}

void Character::setTile(Tile location){
    currentTile = location;
}

void Character::mutateGold(int value){
    goldPossessed += value;
}

bool Character::isDead(){
    return curHP == 0;
}

