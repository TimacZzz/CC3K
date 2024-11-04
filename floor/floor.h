#ifndef FLOOR
#define FLOOR

#include <vector>
#include <iostream>
#include <fstream>
#include "chamber.h"


using namespace std;

class Enemy;
class Player;
class Potion;
class Gold;

class Floor{
    friend class Tile;
    friend class FloorGenerator;

    private:
        const int NUM_ENEMIES = 20;
        const int NUM_POTIONS = 10;
        const int NUM_GOLD_PILES = 10;
        const string PLAYER_STAIR_COLOR = "44";
        const string ENEMY_COLOR = "41";
        const string GOLD_COLOR = "43";
        const string POTION_COLOR = "42";
        bool is_test_mode = false;

        vector<vector<char>> grid;
        vector<Chamber> chambers;
        vector<Enemy*> enemies;
        vector<Potion*> potions;
        vector<Gold*> gold_piles;
        

        void initializeChambers();
        void spawnPlayers(Player *player);
        void spawnStair();
        void spawnPotion();
        void spawnGold();
        void spawnEnemies();
        
    public:
        Floor();
        Floor(ifstream& in);
        ~Floor();
        void printFloor(ostream &out);
        void initializeFloor(Player *player);

        void addGold(Gold *gold);
        void addGold(Tile tile, char code);
        Gold* getGold(Tile tile);
        void removeGold(Tile tile);

        void addPotion(Potion *potion);
        void addPotion(Tile tile, char code);
        Potion* getPotion(Tile tile);
        void removePotion(Tile tile);

        void addEnemy(Enemy *enemy);
        void addEnemy(Tile tile, char code);
        Enemy* getEnemy(Tile tile);
        void removeEnemy(Tile tile);

        const vector<vector<char>>& getGrid();
        const vector<Enemy*>& getEnemies();
};
#endif
