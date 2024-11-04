#include "wd.h"
#include "../character/player/player.h"

WD::WD(Tile tile): Potion("Potion", -5, true, false, "WD", tile){
}

void WD::notify(Character *player){
    if (player->getRace() == "Drow"){
        player->mutateDef(value * DROW_AMPLIFY);
        return;
    }
    player->mutateDef(value);
}

void WD::triggerPermanentEffect(Character *player){
    return;
}

