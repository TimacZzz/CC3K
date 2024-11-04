#ifndef __BD_H__
#define __BD_H__
#include "potion.h"
class Player;

class BD: public Potion{
    public:
        BD(Tile tile);
        void notify(Character *) override;
        void triggerPermanentEffect(Character *) override;
};

#endif
