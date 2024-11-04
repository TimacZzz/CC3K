#include "enemy.h"
#include "../../util.h"
#include "../../floor/floor.h"
#include "../player/player.h"
#include "merchant.h"
#include <cmath>

char Enemy::getSymbol(){
    return symbol;
}

Enemy::Enemy(int maxHP, int curHP, int initATK, int curATK, int initDEF, int curDEF, int goldPossessed, string race, Tile tile):
    Character{maxHP, curHP, initATK, curATK, initDEF, curDEF, goldPossessed, race, tile}{}

bool Enemy::isInAttackRange(Character* player){
    //East
    if (player->getTile().x == this->getTile().getEast().x
        && player->getTile().y == this->getTile().getEast().y){
        return true;
    }
    //North
    else if (player->getTile().x == this->getTile().getNorth().x
        && player->getTile().y == this->getTile().getNorth().y){
        return true;
    }
    //South
    else if (player->getTile().x == this->getTile().getSouth().x
        && player->getTile().y == this->getTile().getSouth().y){
        return true;
    }
    //West
    else if (player->getTile().x == this->getTile().getWest().x
        && player->getTile().y == this->getTile().getWest().y){
        return true;
    }
    //SouthEast
    else if (player->getTile().x == this->getTile().getEast().getSouth().x
        && player->getTile().y == this->getTile().getEast().getSouth().y){
        return true;
    }
    //SouthWest
    else if (player->getTile().x == this->getTile().getWest().getSouth().x
        && player->getTile().y == this->getTile().getWest().getSouth().y){
        return true;
    }
    //NorthEast
    else if (player->getTile().x == this->getTile().getEast().getNorth().x
        && player->getTile().y == this->getTile().getEast().getNorth().y){
        return true;
    }
    //NorthWest
    else if (player->getTile().x == this->getTile().getWest().getNorth().x
        && player->getTile().y == this->getTile().getWest().getNorth().y){
        return true;
    }
    else{
        return false;
    }
}

void Enemy::attack(Character *player, string& action_message){
    Util x;
    int result = x.getRandomNumber(0, 1);

    if (result == 0){
        //miss
        action_message += "The " + std::string(1, this->getSymbol()) + " missed. ";
        return;
    }else{
        // deal damage
        int damage = ceil((100.0 / (100.0 + player->getDef())) * this->getAtk());
        player->mutateHp(-1 * damage);
        
        action_message += "The " + std::string(1, this->getSymbol()) + " deals " + to_string(damage) + " to the PC (" + to_string(player->getHp()) + " HP). ";
    }
}

void Enemy::takeTurn(Floor &curFloor, string& action_message, Player* player){
    // only move an enemy once per turn
    if(moved){
        return;
    }

    if(this->isInAttackRange(player)){
        // make enemy attack player
        if(this->getRace() == "Merchant" && !Merchant::is_hostile){
            moved = false;
        }else{
            this->attack(player, action_message);
            moved = true;
        }
    }

    if(!moved){
        if (Enemy::pause_enemy_move == true){
            return;
        }
        else{
            this->move(curFloor);
            moved = true;
        }
    }
}

void Enemy::move(Floor &curFloor) {
    vector<Tile> neighbors = this->getTile().getEmptyNeighbors(curFloor);
    if(neighbors.size() == 0){
        return;
    }

    // pick a random neighbor
    Tile destination = neighbors[rand() % neighbors.size()];

    // move
    this->getTile().setValue(curFloor, '.');
    this->setTile(destination);
    this->getTile().setValue(curFloor, symbol);
}


void Enemy::attach(Potion *p){
    return;
}

void Enemy::detach(){
    return;
}


Enemy::~Enemy(){}

void Enemy::setMoved(bool value){
    moved = value;
}

bool Enemy::getMoved(){
    return moved;
}

int Enemy::getAtk(){
    return curATK;
}
