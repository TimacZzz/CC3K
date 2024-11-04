#ifndef __RH_H__
#define __RH_H__
#include "potion.h"
class Player;
class RH: public Potion{
    public:
        RH(Tile tile);
        void notify(Character *player) override;
        void triggerPermanentEffect(Character *player) override;
};

#endif
