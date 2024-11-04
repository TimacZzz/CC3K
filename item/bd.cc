#include "bd.h"
#include "../character/player/player.h"

BD::BD(Tile tile): Potion("Potion", 5, true, false, "BD", tile){
}

void BD::notify(Character *player){
    if (player->getRace() == "Drow"){
        player->mutateDef(value * DROW_AMPLIFY);
        return;
    }
    player->mutateDef(value);
}
void BD::triggerPermanentEffect(Character *player){
    return;
}

