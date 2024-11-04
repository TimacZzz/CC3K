#ifndef __BA_H__
#define __BA_H__
#include "potion.h"
class Player;

class BA: public Potion{
    public:
        BA(Tile tile);
        void notify(Character *player) override;
        void triggerPermanentEffect(Character *player) override;
};

#endif
