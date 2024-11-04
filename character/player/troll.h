#ifndef __TROLL_H__
#define __TROLL_H__
#include "player.h"

class Troll : public Player
{
    int self_healing;

public:
    Troll(Tile tile);
    void takeTurn(Floor &curFloor, string& action_message, string command1, string command2 = "") override;
};

#endif
