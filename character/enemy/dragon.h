#ifndef __DRAGON_H__
#define __DRAGON_H__
#include "enemy.h"

class Gold;

class Dragon: public Enemy{
    private: 
        Gold* gold;
        void move(Floor &curFloor) override;
        bool isInAttackRange(Character* player) override;
    public:
        Dragon(Tile tile, Gold* gold);
        void setGold(Gold* gold);
        Gold* getGold();
};

#endif
