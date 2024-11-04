#include "wa.h"
#include "../character/player/player.h"

WA::WA(Tile tile): Potion("Potion", -5, true, false, "WA", tile){
}

void WA::notify(Character *player){
    if (player->getRace() == "Drow"){
        player->mutateAtk(value * DROW_AMPLIFY);
        return;
    }
    player->mutateAtk(value);
}

void WA::triggerPermanentEffect(Character *player){
    return;
}

