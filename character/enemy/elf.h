#ifndef __ELF_H__
#define __ELF_H__
#include "enemy.h"

class Elf: public Enemy{
    public:
        Elf(Tile tile);
        void attack(Character *player, string& action_message) override;
};

#endif
