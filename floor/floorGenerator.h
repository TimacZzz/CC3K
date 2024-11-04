#ifndef FLOORGENERATOR_H
#define FLOORGENERATOR_H

#include <vector>
#include <set>


using namespace std;

class Floor;
class Chamber; 
class Tile;

class FloorGenerator{
    private:
        const int FLOOR_COLS = 79;
        const int FLOOR_ROWS = 25;

        const int MAX_ROOMS_PER_FLOOR;
        const int MIN_ROOMS_PER_FLOOR;
        const int MIN_ROOM_WIDTH;
        const int MAX_ROOM_WIDTH;
        const int MIN_ROOM_HEIGHT;
        const int MAX_ROOM_HEIGHT;
        vector<Chamber> chambers;

        void init(Floor& floor);
        bool canBeValidRoad(Tile& tile, const Floor& floor);
        bool placeRoomOnFloor(int room_width, int room_height, int room_x, int room_y, Floor& floor);
        bool generatePassage(Tile current, Tile end, set<Tile> &visited, Floor &floor, 
            bool guarantee_passable);

    public:
        FloorGenerator(int max_room_count, int min_room_count, int max_room_width, int min_room_width, 
                int max_room_height, int min_room_height);
        void generateRandomFloor(Floor& floor);
        
};


#endif
