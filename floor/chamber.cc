#include "chamber.h"
#include <vector>
#include "../util.h"

using namespace std;

Chamber::Chamber(vector<Tile> tiles): tiles{tiles}{}

Chamber::Chamber(int room_x, int room_y, int room_width, int room_height){
    for(int y = room_y + 1; y < room_y + room_height - 1; ++y){
        for(int x = room_x + 1; x < room_x + room_width - 1; ++x){
            this->tiles.push_back(Tile(x, y));
        }
    }
}

Tile Chamber::getRandomTile() const{
    Util util;
    return this->tiles[util.getRandomNumber(0, this->tiles.size() - 1)];
}

Tile Chamber::getRandomEmptyTile(const Floor& floor) const{
    Util util;

    vector<Tile> empty_tiles;
    for(Tile t : this->tiles){
        if(t.getValue(floor) == '.'){
            empty_tiles.push_back(t);
        }
    }

    int index = util.getRandomNumber(0, empty_tiles.size() - 1);
    return empty_tiles[index];
};

bool Chamber::isInChamber(const char symbol, const Floor& floor) const{
    for(Tile tile: this->tiles){
        if(tile.getValue(floor) == symbol){
            return true;
        }
    }
    return false;
}

bool Chamber::hasEmptyTiles(const Floor& floor) const{
    for(Tile tile: this->tiles){
        if(tile.getValue(floor) == '.'){
            return true;
        }
    }
    return false;
}

vector<Tile> Chamber::getTiles() const{
    return this->tiles;
}

