#include "orc.h"
#include "../../util.h"
#include <cmath>

Orc::Orc(Tile tile): Enemy{180, 180, 30, 30, 25, 25, 0, "Orc", tile}{
    symbol = 'O';
    goblin_slayer_buff = 1.5;
}

void Orc::attack(Character *player, string& action_message){
    int attack = curATK;
    // Check if the Player is a goblin
    if(player->getRace() == "Goblin"){
        curATK *= goblin_slayer_buff;
    }
    Enemy::attack(player, action_message);
    curATK = attack;
}
