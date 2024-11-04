#include "ba.h"
#include "../character/player/player.h"

BA::BA(Tile tile): Potion("Potion", 5, true, false, "BA", tile){
}

void BA::notify(Character *player){
    if (player->getRace() == "Drow"){
        player->mutateAtk(value * DROW_AMPLIFY);
        return;
    }
    player->mutateAtk(value);
}

void BA::triggerPermanentEffect(Character *player){
    return;
}

