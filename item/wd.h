#ifndef __WD_H__
#define __WD_H__

#include "potion.h"
class Player;

class WD: public Potion{
    public:
        WD(Tile tile);
        void notify(Character *player) override;
        void triggerPermanentEffect(Character *player) override;
};

#endif
