#ifndef __GOBLIN_H__
#define __GOBLIN_H__
#include "player.h"


class Goblin: public Player{
    int steal_Gold = 5;
    public:
        Goblin(Tile tile);
        void attack(Character *enemy, string& action_message) override;
};

#endif
