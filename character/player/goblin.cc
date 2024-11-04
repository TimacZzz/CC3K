#include "goblin.h"
#include <cmath>

Goblin::Goblin(Tile tile): Player{110, 110, 15, 15, 20, 20, 0, "Goblin", tile}{
    prevchar = '.';
    score = 0;
}

void Goblin::attack(Character *enemy, string& action_message){
    Player::attack(enemy, action_message);
    if (enemy->isDead()){
        goldPossessed += steal_Gold;
        score += steal_Gold;
        action_message += "PC loots 5 extra gold due to its ability. ";
    }
    return;
}
