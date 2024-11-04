#ifndef __GOLD_H__
#define __GOLD_H__
#include "item.h"

class Gold: public Item{
    string name;
    public:
        Gold(string name, int value, bool canWalk, bool canPick, Tile tile);
        int getValue();
        void setValue(int value);
        string getName();
        void setName(string name);
};

#endif
