#include <iostream>
#include <vector>
#include <stack>
#include <set>
#include <queue>
#include "floorGenerator.h"
#include "floor.h"
#include "tile.h"
#include "../util.h"

using namespace std;

/*
Idea from: https://www.tomstephensondeveloper.co.uk/post/creating-simple-procedural-dungeon-generation

Implementation:
1. Generate random room count, room dimensions, room positions
2. Attempt to place the rooms on the floor
3. If after certain number of attempts the room can't be placed, then back to step 1
4. Repeat step 2 and 3 until room count is reached 
5. Generate passage between rooms using DFS and backtracking. 

The algorithm will try to reduce the number of doors generated, but it will not attempt to minimize it
*/

// reset and initialize the floor and the generator to be ready to generate a new floor
void FloorGenerator::init(Floor &floor) {
    // clear generated chambers
    chambers.clear();

    // clear the grids in the floor
    floor.grid.clear();

    // initialize the floor
    for(int i = 0; i < FLOOR_ROWS; ++i){
        floor.grid.push_back(vector<char>(FLOOR_COLS));

        for (int j = 0; j < FLOOR_COLS; ++j){
            if(j == 0 || j == FLOOR_COLS - 1){
                // generate the walls
                floor.grid[i][j] = '|';
            }else if(i == 0 || i == FLOOR_ROWS - 1){
                // generate the walls
                floor.grid[i][j] = '-';
            }else{
                // fill the inside
                floor.grid[i][j] = ' ';
            }
        }
    }
}

// check if the tile can be turned into something the player can pass through
bool FloorGenerator::canBeValidRoad(Tile& tile, const Floor& floor) {
    // check for out of bounds 
    if(!tile.isInBounds(floor)) {
        return false;
    }

    // if the tile is already walkable, return true 
    if(tile.isWalkable(floor)){
        return true;
    }

    // check for walls
    // a wall eligible to be a door only if it's surrounded by the same type of walls
    vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    if(tile.getValue(floor) == '|'){
        if(tile.getNorth().getValue(floor) == '|' && tile.getSouth().getValue(floor) == '|'){
            return true;
        }
        return false;
    }

    if(tile.getValue(floor) == '-'){
        if(tile.getWest().getValue(floor) == '-' && tile.getEast().getValue(floor) == '-'){
            return true;
        }
        return false;
    }

    // check for adjacent passages, a tile is eligible to be a passage if it 
    // doesn't create a square with other passages
    if(tile.getNorth().getWest().getValue(floor) == '#'
        && tile.getNorth().getValue(floor) == '#'
        && tile.getWest().getValue(floor) == '#'){
        return false;
    }

    if(tile.getNorth().getEast().getValue(floor) == '#'
        && tile.getNorth().getValue(floor) == '#'
        && tile.getEast().getValue(floor) == '#'){
        return false;
    }
    if(tile.getSouth().getWest().getValue(floor) == '#'
        && tile.getSouth().getValue(floor) == '#'
        && tile.getWest().getValue(floor) == '#'){
        return false;
    }
    
    if(tile.getSouth().getEast().getValue(floor) == '#'
        && tile.getSouth().getValue(floor) == '#'
        && tile.getEast().getValue(floor) == '#'){
        return false;
    }

    return true;
}

/*
    place a room on the floor at the specified location
    returns true if the room was placed successfully
*/
bool FloorGenerator::placeRoomOnFloor(int room_width, int room_height, int room_x, int room_y, Floor &floor) {
    // check for room out of bounds
    if(room_x + room_width > FLOOR_COLS || room_y + room_height > FLOOR_ROWS){
        return false;
    }

    // check for room collision, here we make sure that the rooms
    // are at least 1 cell apart (leave room for passages)
    for(int i = room_y - 1; i < room_y + room_height + 1; ++i){
        for(int j = room_x - 1; j < room_x + room_width + 1; ++j){
            if(floor.grid[i][j] != ' '){
                return false;
            }
        }
    }

    // place the room and walls
    for(int i = room_y; i < room_y + room_height; ++i){
        for(int j = room_x; j < room_x + room_width; ++j){
            if(j == room_x || j == room_x + room_width - 1){
                // place the walls
                floor.grid[i][j] = '|';
            }else if(i == room_y || i == room_y + room_height - 1){
                // place the walls
                floor.grid[i][j] = '-';
            }else{
                // place the inside
                floor.grid[i][j] = '.';
            }
        }
    }

    return true;
}

/*
    Generates a passage from start to end coordinates, allowing passage through rooms
    Returns true if the passage was generated successfully
    
    Param guarantee_passable ensures that a passage connecting two tiles will be constructed,
        at the cost of the passage being messy (unnecessary doors or passages). 
        Therefore, it should only be set to true when running this function without 
        guarantee_passable = true returns false
*/
bool FloorGenerator::generatePassage(Tile current, Tile end, set<Tile> &visited, Floor &floor, 
    bool guarantee_passable) {

    // found the tile!
    if(current.x == end.x && current.y == end.y) {
        return true;
    }

    // skip visited tiles
    if(visited.find(current) != visited.end()) {
        return false;
    }

    visited.insert(current);

    // check if the current tile is passable
    if(!canBeValidRoad(current, floor)) {
        return false;
    }

    // remember the value of the current tile
    char originalTileValue = current.getValue(floor);

    if(current.isWall(floor)) {
        // if the tile is a wall, make it a door
        current.setValue(floor, '+');
    } else if (current.getValue(floor) == ' ') {
        // if the tile is an empty space, make it a passage
        current.setValue(floor, '#');
    }

    // First visit tiles that leads to the end point
    bool found_passage = false;
    int xDifference = end.x - current.x;
    int yDifference = end.y - current.y;
    
    if(yDifference > 0) {
        found_passage = generatePassage(current.getSouth(), end, visited, floor, guarantee_passable);
    } else if(yDifference < 0) {
        found_passage = generatePassage(current.getNorth(), end, visited, floor, guarantee_passable);
    }

    if(found_passage) return true;
    
    if(xDifference > 0) {
        found_passage = generatePassage(current.getEast(), end, visited, floor, guarantee_passable);
    } else if(xDifference < 0) {
        found_passage = generatePassage(current.getWest(), end, visited, floor, guarantee_passable);
    }

    if(found_passage) return true;

    // Then visit other possible tiles
    if(yDifference > 0) {
        found_passage = generatePassage(current.getNorth(), end, visited, floor, guarantee_passable);
    }else if(yDifference < 0) {
        found_passage = generatePassage(current.getSouth(), end, visited, floor, guarantee_passable);
    }

    if(found_passage) return true;
    
    if(xDifference > 0) {
        found_passage = generatePassage(current.getWest(), end, visited, floor, guarantee_passable);
    }else if(xDifference < 0) {
        found_passage = generatePassage(current.getEast(), end, visited, floor, guarantee_passable);
    }

    if(found_passage) return true;

    // can't find a passage through the current tile when maintaining the x or y difference to 0
    // Attempt all directions to find a passage (This will lead to extra doors and passages)
    if(guarantee_passable){
        found_passage = generatePassage(current.getNorth(), end, visited, floor, guarantee_passable);
        if(found_passage) return true;
        found_passage = generatePassage(current.getSouth(), end, visited, floor, guarantee_passable);
        if(found_passage) return true;
        found_passage = generatePassage(current.getWest(), end, visited, floor, guarantee_passable);
        if(found_passage) return true;
        found_passage = generatePassage(current.getEast(), end, visited, floor, guarantee_passable);
        if(found_passage) return true;
    }
    
    // could not generate passage, restore the original tile
    current.setValue(floor, originalTileValue);
    return false;
}



// ctor
FloorGenerator::FloorGenerator(int max_room_count, int min_room_count, int max_room_width, 
    int min_room_width, int max_room_height, int min_room_height):
    MAX_ROOMS_PER_FLOOR{max_room_count}, MIN_ROOMS_PER_FLOOR{min_room_count}, 
    MAX_ROOM_WIDTH{max_room_width}, MIN_ROOM_WIDTH{min_room_width}, 
    MAX_ROOM_HEIGHT{max_room_height}, MIN_ROOM_HEIGHT{min_room_height}{}


// randomly populate the given floor
void FloorGenerator::generateRandomFloor(Floor& floor) {
    Util util;
    // reset the generator and floor
    init(floor); 

    int chamber_count = util.getRandomNumber(MIN_ROOMS_PER_FLOOR, MAX_ROOMS_PER_FLOOR);

    const int MAX_ATTEMPS = 20;
    int attempts = 0;
    
    // Limiting attempts in case the rooms generated are impossible to place
    while(attempts <= MAX_ATTEMPS){
        // generate random room dimensions
        int room_width = util.getRandomNumber(MIN_ROOM_WIDTH, MAX_ROOM_WIDTH);
        int room_height = util.getRandomNumber(MIN_ROOM_HEIGHT, MAX_ROOM_HEIGHT);

        const int MAX_ROOM_ATTEMPTS = 15;

        // generate different room positions
        for(int count = 0; count < MAX_ROOM_ATTEMPTS; ++count){
            // the upper left corner of the room
            int room_x = util.getRandomNumber(1, FLOOR_COLS - room_width - 2);
            int room_y = util.getRandomNumber(1, FLOOR_ROWS - room_height - 2);

            if(placeRoomOnFloor(room_width, room_height, room_x, room_y, floor)){
                chambers.push_back(Chamber(room_x, room_y, room_width, room_height));
                break;
            }
        }

        if(chambers.size() == chamber_count){
            break;
        }
        ++attempts;      
    }

    if(attempts > MAX_ATTEMPS){
        // generate random floor from scratch
        generateRandomFloor(floor);
    }else{        
        // generate random passages
        for(int i = 1; i < chambers.size(); ++i){
            Tile tile1 = chambers[i].getRandomTile();
            Tile tile2 = chambers[i - 1].getRandomTile();

            set<Tile> visited;

            // try to generate passages between the two tiles
            bool res = generatePassage(tile1, tile2, visited, floor, false);
            if(!res){
                // if the passage cannot be generated, turn on the guarantee passable flag
                res = generatePassage(tile1, tile2, visited, floor, true);
                if(!res){
                    // if the passage still cannot be generated 
                    // (This should never happen if the algorithm is correct),generate a new floor
                    generateRandomFloor(floor);
                }
            }
        }
    
    }

}


