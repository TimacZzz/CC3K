#include "elf.h"
#include <cmath>
#include "../../util.h"

Elf::Elf(Tile tile): Enemy{140, 140, 30, 30, 10, 10, 0, "Elf", tile}{
    symbol = 'E';
}

// elf has two attacks against every race other than drow
void Elf::attack(Character *player, string& action_message){
    Enemy::attack(player, action_message);
    if (player->getRace() != "Drow"){
        Enemy::attack(player, action_message);
    }
    return;
}
