#ifndef __WA_H__
#define __WA_H__

#include "potion.h"
class Player;

class WA: public Potion{
    public:
        WA(Tile tile);
        void notify(Character *player) override;
        void triggerPermanentEffect(Character *player) override;
};

#endif
