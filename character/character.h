#ifndef __CHARACTER_H__
#define __CHARACTER_H__
#include <string>
#include <vector>
using namespace std;
#include "../floor/tile.h"

class Potion;

class Character{
    protected:
        int maxHP, curHP, initATK, curATK, initDEF, curDEF, goldPossessed;
        Tile currentTile;
        vector<Potion *> observers;
        string race;
    public:
        Character(int maxHP, int curHP, int initATK, int curATK, int initDEF, int curDEF, int goldPossess, string race, Tile tile);
        int getDef();
        virtual int getAtk();
        int getHp();
        int getGold();
        void mutateHp(int value);
        void mutateAtk(int value);
        void mutateDef(int value);
        void mutateMaxHp(int value);
        void mutateGold(int value);
        void setTile(Tile location);
        bool isDead();
        Tile getTile();
        string getRace();
        virtual void attack(Character *, string& action_message) = 0;
        virtual void attach(Potion *p) = 0;
        virtual void detach() = 0;
        virtual ~Character();
};

#endif
