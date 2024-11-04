#include "rh.h"
#include "../character/player/player.h"

RH::RH(Tile tile): Potion("Potion", 10, true, false, "RH", tile){
}

void RH::notify(Character *player){
    return;
}

void RH::triggerPermanentEffect(Character *player){
    if (player->getRace() == "Drow"){
        player->mutateHp(value * DROW_AMPLIFY);
        return;
    }
    player->mutateHp(value);
}

