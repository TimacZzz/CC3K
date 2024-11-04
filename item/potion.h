#ifndef __POTION_H__
#define __POTION_H__
#include <string>
#include "item.h"
using namespace std;

class Character;

class Potion: public Item{
    protected:
        string name;
        //Field designed to amplify the effect of the potion if the Player is a Droww
        const float DROW_AMPLIFY = 1.5;
    public:
        //Static fields storing the boolean value of whether the six potions have been learned by the Player or not
        inline static bool rh_learned = false;
        inline static bool ph_learned = false;
        inline static bool wa_learned = false;
        inline static bool wd_learned = false;
        inline static bool ba_learned = false;
        inline static bool bd_learned = false;
        
        Potion(string type, int value, bool canWalk, bool canPick, string name, Tile tile);
        virtual void notify(Character *player) = 0;
        virtual void triggerPermanentEffect(Character *player) = 0;
        string getName();
        int getPrice();
        bool isLearned();
        void learnPotion(string name);
        virtual ~Potion();
};

#endif
