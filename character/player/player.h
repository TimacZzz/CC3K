#ifndef __PLAYER_H__
#define __PLAYER_H__
#include <string>
#include <vector>
#include <iostream>

#include "../character.h"

class Item;
class Potion;
class Gold;
class Merchant;
class Floor;

class Player: public Character {
    protected:
        int score;
        char prevchar;
        vector<Potion *> inventory;

        
        void storePotion(Potion *p);
        void usePotionInInventory(int value);
        void consumePotion(Potion *p, string& action_message);
        void move(Floor &curFloor, string& action_message, string command);
        
    public:
        Player(int maxHP, int curHP, int initATK, int curATK, int initDEF, int curDEF, int goldPossessed, string race, Tile tile);
        virtual void attack(Character *enemy, string& action_message);
        void attach(Potion *p) override;
        void detach() override;
        virtual void takeTurn(Floor &curFloor, string& action_message, string command1 = "", string command2 = "");
        void pickGold(int value);
        virtual int getScore();
        char getPrevChar();
        void setPrevChar(char value);
        int getInventorySize();
        void printInventory(string &action_message);
        void buyItem(Merchant *merchant, int position, string &action_message);
        ~Player();
};

#endif
