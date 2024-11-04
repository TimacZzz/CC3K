#include "troll.h"
#include <cmath>

Troll::Troll(Tile tile): Player{120, 120, 25, 25, 15, 15, 0, "Troll", tile}{
    self_healing = 5;
    prevchar = '.';
}

void Troll::takeTurn(Floor &curFloor, string& action_message, string command1, string command2) {
    action_message += "PC healed " + to_string(self_healing) + " HP due to its ability. ";
    Player::takeTurn(curFloor, action_message, command1, command2);
    this->mutateHp(self_healing);
}

