#ifndef __ITEM_H__
#define __ITEM_H__
#include <string>
#include "../floor/tile.h"

class Item{
    protected:
        Tile currentTile;
        string type;
        int value;
        bool canWalk;
        bool canPick; 
    public:
        Item(string type, int value, bool canWalk, bool canPick, Tile tile);
        void setTile(Tile location);
        void setCanWalk(bool value);
        void setCanPick(bool value);
        Tile getTile();
        bool getCanWalk();
        bool getCanPick();
        string getType();
};

#endif
