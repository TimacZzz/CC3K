#ifndef __MERCHANT_H__
#define __MERCHANT_H__
#include "enemy.h"
#include <vector>

class Potion;

class Merchant: public Enemy{
    public:
        vector<Potion *>storage;
        inline static bool is_hostile = false;
        Merchant(Tile tile);
        ~Merchant();
        void printStorage(string &action_message);
        void sellItem(int position);
};

#endif
