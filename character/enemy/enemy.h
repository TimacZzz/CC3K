#ifndef __ENEMY_H__
#define __ENEMY_H__
#include <string>
#include "../character.h"

class Player;
class Potion;
class Floor;

class Enemy: public Character {
    protected:
        char symbol;
        bool moved = false;

        virtual void move(Floor &curFloor);
        virtual bool isInAttackRange(Character* player);
    public:
        inline static bool pause_enemy_move = false;

        Enemy(int maxHP, int curHP, int initATK, int curATK, int initDEF, int curDEF, int goldPossessed, string race, Tile tile);
        virtual void attack(Character *player, string& action_message);
        virtual void takeTurn(Floor &curFloor, string& action_message, Player* player);
        char getSymbol();
        bool getMoved();
        void setMoved(bool value);
        int getAtk() override;
        void attach(Potion *p) override;
        void detach() override;

        ~Enemy();
};

#endif
