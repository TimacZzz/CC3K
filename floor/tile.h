#ifndef TILE
#define TILE

#include <vector>
#include <string>

using namespace std;

class Floor;
class Tile{
    public:
        int x;
        int y;

        Tile(int x, int y);
        
        char getValue(const Floor& floor) const;

        void setValue(Floor& floor, char value) const;

        bool isInBounds(const Floor& floor) const;

        bool isWalkable(const Floor& floor) const;

        bool isWall(const Floor& floor) const;

        bool isPlayer(const Floor& floor) const;

        bool isEnemy(const Floor& floor) const;

        bool isStair(const Floor& floor) const;

        bool isPotion(const Floor& floor) const;

        bool isGold(const Floor& floor) const;

        // get neighbors of the tile that are floors "."
        vector<Tile> getEmptyNeighbors(const Floor& floor) const;

        vector<Tile> getNeighbors(const Floor& floor) const;

        static Tile getTileInDirection(const Tile& tile, const string& direction);

        Tile getNorth() const;

        Tile getSouth() const;

        Tile getWest() const;

        Tile getEast() const;

        bool operator<(const Tile& other) const;

        bool operator==(const Tile& other) const;
};

#endif 
