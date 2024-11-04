#include "dragon.h"
#include "../../item/gold.h"

Dragon::Dragon(Tile tile, Gold* gold): Enemy{150, 150, 20, 20, 20, 20, 0, "Dragon", tile}, gold{gold}{
    symbol = 'D';
}

Gold* Dragon::getGold(){
    return gold;
}

void Dragon::setGold(Gold* gold){
    this->gold = gold;
}

void Dragon::move(Floor &curFloor){
    return;
}

// Dragon also checks the eight tiles around the Gold Pile it's guarding.
// If the Player enters the range, the Dragon will attack with its dragon breath
bool Dragon::isInAttackRange(Character* player){
    if(Enemy::isInAttackRange(player)){
        return true;
    }

    //East
    if (player->getTile() == gold->getTile().getEast()){
        return true;
    }
    //North
    else if (player->getTile() == gold->getTile().getNorth()){
        return true;
    }
    //South
    else if (player->getTile() == gold->getTile().getSouth()){
        return true;
    }
    //West
    else if (player->getTile() == gold->getTile().getWest()){
        return true;
    }
    //SouthEast
    else if (player->getTile() == gold->getTile().getEast().getSouth()){
        return true;
    }
    //SouthWest
    else if (player->getTile() == gold->getTile().getWest().getSouth()){
        return true;
    }
    //NorthEast
    else if (player->getTile() == gold->getTile().getEast().getNorth()){
        return true;
    }
    //NorthWest
    else if (player->getTile() == gold->getTile().getWest().getNorth()){
        return true;
    }
    else{
        return false;
    }
}
