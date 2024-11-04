#ifndef __SHADE_H__
#define __SHADE_H__
#include "player.h"

class Shade: public Player{
    public:
        Shade(Tile tile);
        int getScore() override;
};

#endif
