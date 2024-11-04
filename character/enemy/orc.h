#ifndef __ORC_H__
#define __ORC_H__
#include "enemy.h"

class Orc: public Enemy{
    float goblin_slayer_buff;
    public:
        Orc(Tile tile);
        void attack(Character *player, string& action_message) override;
};

#endif
