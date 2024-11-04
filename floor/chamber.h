#ifndef CHAMBER
#define CHAMBER

#include <vector>
#include "tile.h"

using namespace std;

class Floor;

class Chamber{
    private:
        vector<Tile> tiles;

    public:
        Chamber(vector<Tile> tiles);

        // initializes a rectangular chamber (only used in the floorGenerator)
        Chamber(int room_x, int room_y, int room_width, int room_height);

        // get a random Tile in the chamber
        Tile getRandomTile() const;

        // get a random, non-occupied Tile in the chamber
        Tile getRandomEmptyTile(const Floor& floor) const;

        // check if certain character or item is in this chamber
        bool isInChamber(const char symbol, const Floor& floor) const;

        // check if a chamber still has empty tiles
        bool hasEmptyTiles(const Floor& floor) const;

        vector<Tile> getTiles() const;
};
#endif
