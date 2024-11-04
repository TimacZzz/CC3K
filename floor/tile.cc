#include "tile.h"
#include "floor.h"

Tile::Tile(int x, int y):x{x}, y{y}{}

// get the value of the tile in the given floor
char Tile::getValue(const Floor& floor) const{
    return floor.grid[y][x];
}

// set the value of the tile in the given floor
void Tile::setValue(Floor& floor, char value) const {
    floor.grid[y][x] = value;
}

// check if the tile is in the boundary of the floor
bool Tile::isInBounds(const Floor& floor) const {
    return x >= 0 && x < floor.grid[0].size() && y >= 0 && y < floor.grid.size();
}

// check if the tile is walkable if empty
bool Tile::isWalkable(const Floor& floor) const {
    return getValue(floor) == '#' || getValue(floor) == '+' || getValue(floor) == '.';
}

// check if the tile is a wall
bool Tile::isWall(const Floor& floor) const {
    return getValue(floor) == '|' || getValue(floor) == '-';
}

// check if the tile is a player
bool Tile::isPlayer(const Floor& floor) const {
    return getValue(floor) == '@';
}

// check if the tile is an enemy
bool Tile::isEnemy(const Floor& floor) const {
    return getValue(floor) == 'H' || getValue(floor) == 'W' || getValue(floor) == 'E' ||
            getValue(floor) == 'O' || getValue(floor) == 'M' || getValue(floor) == 'D' ||
            getValue(floor) == 'L';
}

// check if the tile is a stair
bool Tile::isStair(const Floor& floor) const {
    return getValue(floor) == '\\';
}

// check if the tile is a potion
bool Tile::isPotion(const Floor& floor) const {
    return getValue(floor) == 'P';
}

// check if the tile is a gold pile
bool Tile::isGold(const Floor& floor) const {
    return getValue(floor) == 'G';
}

// get the neighbors of the tile
vector<Tile> Tile::getEmptyNeighbors(const Floor& floor) const {
    vector<Tile> neighbors;

    Tile north = getNorth();
    Tile south = getSouth();
    Tile west = getWest();
    Tile east = getEast();
    Tile northwest = getNorth().getWest();
    Tile northeast = getNorth().getEast();
    Tile southeast = getSouth().getEast();
    Tile southwest = getSouth().getWest();

    if (north.isInBounds(floor) && north.getValue(floor) == '.') {
        neighbors.push_back(north);
    } 
    if (south.isInBounds(floor) && south.getValue(floor) == '.') {
        neighbors.push_back(south);
    }
    if (west.isInBounds(floor) && west.getValue(floor) == '.') {
        neighbors.push_back(west);
    }
    if (east.isInBounds(floor) && east.getValue(floor) == '.') {
        neighbors.push_back(east);
    }
    if (northwest.isInBounds(floor) && northwest.getValue(floor) == '.') {
        neighbors.push_back(northwest);
    }
    if (northeast.isInBounds(floor) && northeast.getValue(floor) == '.') {
        neighbors.push_back(northeast);
    }
    if (southeast.isInBounds(floor) && southeast.getValue(floor) == '.') {
        neighbors.push_back(southeast);
    }
    if (southwest.isInBounds(floor) && southwest.getValue(floor) == '.') {
        neighbors.push_back(southwest);
    }
    return neighbors;
}

// get the neighbors of the tile
vector<Tile> Tile::getNeighbors(const Floor& floor) const {
    Tile north = getNorth();
    Tile south = getSouth();
    Tile west = getWest();
    Tile east = getEast();
    Tile northwest = getNorth().getWest();
    Tile northeast = getNorth().getEast();
    Tile southeast = getSouth().getEast();
    Tile southwest = getSouth().getWest();
    
    vector<Tile> neighbors;
    if(north.isInBounds(floor)){
        neighbors.push_back(north);
    }
    if(south.isInBounds(floor)){
        neighbors.push_back(south);
    }
    if(west.isInBounds(floor)){
        neighbors.push_back(west);
    }
    if(east.isInBounds(floor)){
        neighbors.push_back(east);
    }
    if(northwest.isInBounds(floor)){
        neighbors.push_back(northwest);
    }
    if(northeast.isInBounds(floor)){
        neighbors.push_back(northeast);
    }
    if(southeast.isInBounds(floor)){
        neighbors.push_back(southeast);
    }
    if(southwest.isInBounds(floor)){
        neighbors.push_back(southwest);
    }

    return neighbors;
}

// find the tile in the given direction
Tile Tile::getTileInDirection(const Tile& tile, const string& direction){
    if(direction == "no"){
        return tile.getNorth();
    }else if(direction == "so"){
        return tile.getSouth();
    }else if(direction == "ea"){
        return tile.getEast();
    }else if(direction == "we"){
        return tile.getWest();
    }else if(direction == "ne"){
        return tile.getNorth().getEast();
    }else if(direction == "nw"){
        return tile.getNorth().getWest();
    }else if(direction == "se"){
        return tile.getSouth().getEast();
    }else{
        return tile.getSouth().getWest();
    }
}


// get the north tile of the tile
Tile Tile::getNorth() const {
    return Tile{x, y - 1};
}

// get the south tile of the tile
Tile Tile::getSouth() const {
    return Tile{x, y + 1};
}

// get the west tile of the tile
Tile Tile::getWest() const {
    return Tile{x - 1, y};
}

// get the east tile of the tile
Tile Tile::getEast() const {
    return Tile{x + 1, y};
}

bool Tile::operator<(const Tile& other) const {
    return x < other.x || (x == other.x && y < other.y);
}

bool Tile::operator==(const Tile& other) const {
    return x == other.x && y == other.y;
}
