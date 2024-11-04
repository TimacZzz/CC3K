#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include "floor.h"
#include "../util.h"
#include "../character/player/player.h"
#include "../item/gold.h"
#include "../character/enemy/dragon.h"
#include "../character/enemy/halfling.h"
#include "../character/enemy/merchant.h"
#include "../character/enemy/orc.h"
#include "../character/enemy/elf.h"
#include "../character/enemy/dwarf.h"
#include "../character/enemy/human.h"
#include "../item/potion.h"
#include "../item/rh.h"
#include "../item/wa.h"
#include "../item/wd.h"
#include "../item/ph.h"
#include "../item/ba.h"
#include "../item/bd.h"

using namespace std;

// const values used to initialize the floor
const vector<char> ENEMY_LIST = {'H', 'H', 'H', 'H', 'W', 'W', 'W', 'L', 'L', 
    'L', 'L', 'L', 'E', 'E', 'O', 'O', 'M', 'M'}; // dragon is not included
const vector<char> POTIONS_LIST = {'0', '1', '2', '3', '4', '5'};
const vector<char> GOLD_LIST = {'6', '6', '6', '6', '6', '9', '7', '7'};

// use BFS to find all chambers
void Floor::initializeChambers(){
    // get a deep copy of the grid
    vector<vector<char>> grid_copy = this->grid;

    // traverse the grid
    for(int y = 0; y < grid_copy.size(); ++y){
        for(int x = 0; x < grid_copy[y].size(); ++x){

            // once located a chamber floor, find all the floors connected to it
            Tile tile = Tile(x, y);
            if(tile.getValue(*this) != '#' && tile.getValue(*this) != '-' && tile.getValue(*this) != '|'
                && tile.getValue(*this) != '+' && tile.getValue(*this) != ' '){
                // use BFS to find all connected floors
                queue<Tile> next_to_travel;
                vector<Tile> tiles_in_chamber;
                next_to_travel.push(tile);

                while(!next_to_travel.empty()){
                    Tile current = next_to_travel.front();
                    next_to_travel.pop();

                    // if the tile is not a floor or symbols used in test mode, continue
                    if(!current.isInBounds(*this) || (current.getValue(*this) == '-' 
                        || current.getValue(*this) == '|' || current.getValue(*this) == '+')){
                        continue;
                    }

                    // if the tile is a symbol used in test mode, set test mode to true
                    if(current.getValue(*this) != '.'){
                        is_test_mode = true;
                    }

                    // mark the tile as visited and add it to chamber
                    current.setValue(*this, '+');
                    tiles_in_chamber.push_back(current);

                    // add adjacent tiles to the queue
                    next_to_travel.push(current.getNorth());
                    next_to_travel.push(current.getSouth());
                    next_to_travel.push(current.getWest());
                    next_to_travel.push(current.getEast());
                }

                // add the chamber to the list
                this->chambers.push_back(Chamber(tiles_in_chamber));
            }
        }
    }

    // reset the grid
    this->grid = grid_copy;
}

// spawns the player in a random chamber and give the tile to the player (only the tile field is changed)
void Floor::spawnPlayers(Player *player){
    // first check if the Player is already given
    if(is_test_mode){
        for(Chamber& chamber: this->chambers){
            vector<Tile> tiles_in_chamber = chamber.getTiles();
            for(Tile& tile: tiles_in_chamber){
                if(tile.isPlayer(*this)){
                    player->setTile(Tile(tile));
                    return;
                }
            }
        }
        return;
    }
    

    // spawn the player in a random valid chamber 
    Util util;
    vector<const Chamber*> valid_chambers;
    for(const Chamber& chamber: this->chambers){
        if(chamber.hasEmptyTiles(*this) && !chamber.isInChamber('\\', *this)){
            valid_chambers.push_back(&chamber);
        }
    }
    
    const Chamber* chamber = valid_chambers[util.getRandomNumber(0, valid_chambers.size() - 1)];
    Tile tile = chamber->getRandomEmptyTile(*this);
    tile.setValue(*this, '@'); 
    player->setTile(Tile(tile));
    return;
}

// spawns the stair in a random chamber
void Floor::spawnStair(){
    // first check if the stair is already given
    if(is_test_mode){
        for(Chamber& chamber: this->chambers){
            vector<Tile> tiles_in_chamber = chamber.getTiles();
            for(Tile& tile: tiles_in_chamber){
                if(tile.isStair(*this)){
                    return; 
                }
            }
        }
    }
    

    // spawn the stair in a random chamber
    Util util;
    vector<const Chamber*> valid_chambers;
    for(const Chamber& chamber: this->chambers){
        if(chamber.hasEmptyTiles(*this) && !chamber.isInChamber('@', *this)){
            valid_chambers.push_back(&chamber);
        }
    }
    
    const Chamber* chamber = valid_chambers[util.getRandomNumber(0, valid_chambers.size() - 1)];
    Tile tile = chamber->getRandomEmptyTile(*this);
    tile.setValue(*this, '\\'); 
}

// spawns potions in random chambers
void Floor::spawnPotion(){
    if(is_test_mode){
        // fist check if potions are already given
        for(Chamber& chamber: this->chambers){
            vector<Tile> tiles_in_chamber = chamber.getTiles();
            for(Tile& tile: tiles_in_chamber){
                if(tile.getValue(*this) == '0' || tile.getValue(*this) == '1' || tile.getValue(*this) == '2'
                    || tile.getValue(*this) == '3' || tile.getValue(*this) == '4' || tile.getValue(*this) == '5'){
                    addPotion(tile, tile.getValue(*this));
                }
            }
        }
        return;
    }

    Util util;
    vector<const Chamber*> valid_chambers;
    int num_potions = 0;
    for(const Chamber& chamber: this->chambers){
        if(chamber.hasEmptyTiles(*this)){
            valid_chambers.push_back(&chamber);
        }
    }
    
    while(num_potions < this->NUM_POTIONS){
        // spawn a potion in a random chamber
        const Chamber* chamber = valid_chambers[util.getRandomNumber(0, valid_chambers.size() - 1)];
        Tile tile = chamber->getRandomEmptyTile(*this);

        // add potion to the list
        int potion_type = util.getRandomNumber(0, POTIONS_LIST.size() - 1);
        addPotion(tile, POTIONS_LIST[potion_type]);
        ++num_potions;
    }
}

// spawns gold piles in random chambers
void Floor::spawnGold(){
    if(is_test_mode){
        // first check if gold is already given
        for(Chamber& chamber: this->chambers){
            vector<Tile> tiles_in_chamber = chamber.getTiles();
            for(Tile& tile: tiles_in_chamber){
                if(tile.getValue(*this) == '6' || tile.getValue(*this) == '7' 
                    || tile.getValue(*this) == '8' || tile.getValue(*this) == '9'){
                    addGold(tile, tile.getValue(*this));
                } 
            }
        }
        return;
    }

    Util util;
    vector<const Chamber*> valid_chambers;
    int num_gold = 0;
    for(const Chamber& chamber: this->chambers){
        if(chamber.hasEmptyTiles(*this)){
            valid_chambers.push_back(&chamber);
        }
    }


    while(num_gold < this->NUM_GOLD_PILES){
        // spawn gold in a random chamber
        const Chamber* chamber = valid_chambers[util.getRandomNumber(0, valid_chambers.size() - 1)];
        Tile tile = chamber->getRandomEmptyTile(*this);

        int gold_type = util.getRandomNumber(0, GOLD_LIST.size() - 1);
        addGold(tile, GOLD_LIST[gold_type]);
        ++num_gold;
    }
}

// spawns enemies in random chambers
void Floor::spawnEnemies(){
    if(is_test_mode){
        for(Chamber& chamber: this->chambers){
            vector<Tile> tiles_in_chamber = chamber.getTiles();
            for(Tile& tile: tiles_in_chamber){
                // generate all other enemies but dragon
                if(tile.getValue(*this) == 'D'){
                    vector<Tile> neighbors = tile.getNeighbors(*this);

                    // find dragon hoard around it
                    bool found = false;
                    for(Tile& neighbor: neighbors){
                        for(Gold* gold: gold_piles){
                            if(gold->getValue() == 6 && gold->getTile() == neighbor){
                                enemies.push_back(new Dragon(tile, gold));   
                                found = true;
                                break;
                            }    
                        }
                        if(found){
                            break;
                        }
                    }
                }else if(tile.isEnemy(*this)){
                    addEnemy(tile, tile.getValue(*this));
                }
            }
        }
        return;
    }

    int num_enemies = 0;
    Util util;

    vector<const Chamber*> valid_chambers;
    for(const Chamber& chamber: this->chambers){
        if(chamber.hasEmptyTiles(*this)){
            valid_chambers.push_back(&chamber);
        }
    }

    for(Gold* gold: gold_piles){
        if(gold->getValue() == 6){
           vector<Tile> neighbors = gold->getTile().getEmptyNeighbors(*this);
            if(neighbors.size() == 0){
               // can't find empty tile to spawn dragon, replace with another random gold pile
               cout << "Dragon hoard at " << gold->getTile().x << ", " << gold->getTile().y << 
               " can not find an empty tile to spawn dragon, it will be replaced by another random gold pile" << endl;


               const vector<int> GOLD_VALUE_LIST = {1, 2};
               int value = GOLD_VALUE_LIST[util.getRandomNumber(0, GOLD_VALUE_LIST.size() - 1)];
               gold->setCanPick(true);
               gold->setCanWalk(true);
               gold->setValue(value);

               if(value == 1){
                   gold->setName("Small Hoard");
               }else if(value == 2){
                   gold->setName("Normal Gold Pile");
               }
            }else{
               // spawn a dragon nearby
               Tile tile = neighbors[util.getRandomNumber(0, neighbors.size() - 1)];
               tile.setValue(*this, 'D');
               enemies.push_back(new Dragon(tile, gold));
            }
        }
    }

    while(num_enemies < this->NUM_ENEMIES){
        // spawn an enemy in a random chamber
        const Chamber* chamber = valid_chambers[util.getRandomNumber(0, valid_chambers.size() - 1)];
        Tile tile = chamber->getRandomEmptyTile(*this);

        int enemy_type = util.getRandomNumber(0, ENEMY_LIST.size() - 1);
        addEnemy(tile, ENEMY_LIST[enemy_type]);
        ++num_enemies; 
    }
}

// spawn needed objects in the floor and locate the chambers
void Floor::initializeFloor(Player *player){
    // locate the chambers
    initializeChambers();

    // spawn needed objects
    spawnPlayers(player);
    spawnStair();
    spawnPotion();
    spawnGold();
    spawnEnemies();
}


// creates an empty floor
Floor::Floor(){}

// reads in a floor from a file, then initializes the floor
Floor::Floor(ifstream& in){
    string line; 
    bool first = true;

    while(in.peek() != EOF){
        getline(in, line);
        vector<char> row;

        // detects whether the line is the last line of the floor
        int vertical_walls = 0;
        bool is_first_or_last = true;
        for(char c: line){
            if(c == '|'){
                ++vertical_walls;
            }else if (c != '-'){
                is_first_or_last = false;
            }
            row.push_back(c);
        }
        if(vertical_walls != 2){
            is_first_or_last = false;
        }
        this->grid.push_back(row);

        // breaks the loop if the line is the last line
        if(is_first_or_last && first){
            first = false;
        }else if(is_first_or_last){
            break;
        }
    }
}

// prints the floor
void Floor::printFloor(ostream& out){
    for(int i = 0; i < this->grid.size(); ++i){
        for(int j = 0; j < this->grid[i].size(); ++j){
            Tile tile = Tile(j, i);

            // colored ASCII output
            if(tile.isPlayer(*this) || tile.isStair(*this)){
                out  << "\033[" << this->PLAYER_STAIR_COLOR << "m" << tile.getValue(*this) << "\033[m";
            }else if(tile.isPotion(*this)){
                out  << "\033[" << this->POTION_COLOR << "m" << tile.getValue(*this) << "\033[m";
            }else if(tile.isGold(*this)){
                out  << "\033[" << this->GOLD_COLOR << "m" << tile.getValue(*this) << "\033[m";
            }else if(tile.isEnemy(*this)){
                out  << "\033[" << this->ENEMY_COLOR << "m" << tile.getValue(*this) << "\033[m";
            }else{
                out  << tile.getValue(*this);
            }
        }
        out  << endl;
    }
}

void Floor::addGold(Gold *gold){
    gold_piles.push_back(gold);
    gold->getTile().setValue(*this, 'G');
}

void Floor::addGold(Tile tile, char code){
    if(code == '6'){
        gold_piles.push_back(new Gold("Normal Gold Pile", 2, true, true, tile));
    }else if(code == '7'){
        gold_piles.push_back(new Gold("Small Hoard", 1, true, true, tile));
    }else if(code == '8'){
        gold_piles.push_back(new Gold("Merchant Hoard", 4, true, true, tile));
    }else if(code == '9'){
        gold_piles.push_back(new Gold("Dragon Hoard", 6, false, false, tile));
    }else{
        return;
    }
    tile.setValue(*this, 'G');
}

Gold* Floor::getGold(Tile tile){
    for(Gold* gold: gold_piles){
        if(gold->getTile() == tile){
            return gold;
        }
    }
    return nullptr;
}

void Floor::removeGold(Tile tile){
    for(int i = 0; i < gold_piles.size(); ++i){
        if(gold_piles[i]->getTile() == tile){
            tile.setValue(*this, '.');
            gold_piles.erase(gold_piles.begin() + i);
            return;
        }
    }
}

void Floor::addPotion(Potion *potion){
    potions.push_back(potion);
    potion->getTile().setValue(*this, 'P');
}

void Floor::addPotion(Tile tile, char code){
    if(code == '0'){
        potions.push_back(new RH(tile));
    }else if(code == '1'){
        potions.push_back(new BA(tile));
    }else if(code == '2'){
        potions.push_back(new BD(tile));
    }else if(code == '3'){
        potions.push_back(new PH(tile));
    }else if(code == '4'){
        potions.push_back(new WA(tile));
    }else if(code == '5'){
        potions.push_back(new WD(tile));
    }else{
        return;
    }

    tile.setValue(*this, 'P');
}

Potion* Floor::getPotion(Tile tile){
    for(Potion* potion: potions){
        if(potion->getTile() == tile){
            return potion;
        }
    }
    return nullptr;
}

void Floor::removePotion(Tile tile){
    for(int i = 0; i < potions.size(); ++i){
        if(potions[i]->getTile() == tile){
            tile.setValue(*this, '.');
            potions.erase(potions.begin() + i);
            return;
        }
    }
}

void Floor::addEnemy(Enemy *enemy){
    enemies.push_back(enemy);
    enemy->getTile().setValue(*this, enemy->getSymbol());
}

void Floor::addEnemy(Tile tile, char code){
    if(code == 'H'){
        enemies.push_back(new Human(tile));
        tile.setValue(*this, 'H');
    }else if(code == 'W'){
        enemies.push_back(new Dwarf(tile));
        tile.setValue(*this, 'W');
    }else if(code == 'E'){
        enemies.push_back(new Elf(tile));
        tile.setValue(*this, 'E');
    }else if(code == 'O'){
        enemies.push_back(new Orc(tile));
        tile.setValue(*this, 'O');
    }else if(code == 'M'){
        enemies.push_back(new Merchant(tile));
        tile.setValue(*this, 'M');
    }else if(code == 'L'){
        enemies.push_back(new Halfling(tile));
    }else{
        return;
    }
    tile.setValue(*this, code);
}

Enemy* Floor::getEnemy(Tile tile){
    for(Enemy* enemy: enemies){
        if(enemy->getTile() == tile){
            return enemy;
        }
    }
    return nullptr;
}

void Floor::removeEnemy(Tile tile){
    for(int i = 0; i < enemies.size(); ++i){
        if(enemies[i]->getTile() == tile){
            tile.setValue(*this, '.');
            enemies.erase(enemies.begin() + i);
            return;
        }
    }
}

const vector<vector<char>>& Floor::getGrid(){
    return this->grid;
}

const vector<Enemy*>& Floor::getEnemies(){
    return this->enemies;
}

Floor::~Floor(){
    for(Enemy* enemy: enemies){
        delete enemy;
    }
    for(Potion* potion: potions){
        delete potion;
    }
    for(Gold* gold: gold_piles){
        delete gold;
    }
}

