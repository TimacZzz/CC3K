#include "ph.h"
#include "../character/player/player.h"

PH::PH(Tile tile): Potion("Potion", -10, true, false, "PH", tile){
}

void PH::notify(Character *player){
    return;
}

void PH::triggerPermanentEffect(Character *player){
    if (player->getRace() == "Drow"){
        player->mutateHp(value * DROW_AMPLIFY);
        return;
    }
    player->mutateHp(value);
}

