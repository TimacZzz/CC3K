#ifndef __PH_H__
#define __PH_H__

#include "potion.h"
class Player;

class PH: public Potion{
    public:
        PH(Tile tile);
        void notify(Character *player) override;
        void triggerPermanentEffect(Character *player) override;
};

#endif
