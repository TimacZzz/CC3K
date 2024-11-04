#include <iostream>
#include <stdexcept>
#include <fstream>
#include "controller.h"
#include "util.h"
#include "character/player/drow.h"
#include "character/player/vampire.h"
#include "character/player/troll.h"
#include "character/player/goblin.h"
#include "character/player/shade.h"
#include "floor/floorGenerator.h"
#include "character/enemy/enemy.h"
#include "character/enemy/dragon.h"
#include "item/gold.h"
#include "character/enemy/merchant.h"
#include "item/potion.h"



using namespace std;

const int RESTART_GAME = 1;
const int QUIT_GAME = 2;
const int CONTINUE_GAME = 3;

// get all the required parameters to initialize the game, auto error checking is included
int Controller::initializeGame(istream& in, ostream& out, const string& map_file){
    Util util;
    string string_command;
    int int_command;
    out << "Welcome to CC3K, this game is made by Zheng Pei and Timac Wang" << endl;

    // select Player race
    auto race_validator = [](string s){return s == "s" || s == "d" || s == "v" 
        || s == "g" || s == "t" || s == "q" || s == "r";};
    util.requestInput<string>(in, out, string_command, "Select your race (s/d/v/g/t/q(quit)/r(restart)):", race_validator);

    if(string_command == "q"){
        return QUIT_GAME;
    }else if(string_command == "r"){
        return RESTART_GAME;
    }else if(string_command == "s"){
        player = new Shade(Tile(0, 0));
    }else if(string_command == "d"){
        player = new Drow(Tile(0, 0));
    }else if(string_command == "v"){
        player = new Vampire(Tile(0, 0));
    }else if(string_command == "g"){
        player = new Goblin(Tile(0, 0));
    }else if(string_command == "t"){
        player = new Troll(Tile(0, 0));
    }

    // select game seed
    auto yes_no_validator = [](string s){return s == "y" || s == "n" || s == "q" || s == "r";};
    util.requestInput<string>(in, out, string_command, 
        "Do you want to customize the game seed (y/n):", yes_no_validator);

    if(string_command == "y"){
        // input seed
        int seed;
        util.requestInput<int>(in, out, seed, "Input an integer value as the seed:");
        util.setRandomSeed(seed);
    }else if (string_command == "n"){
        // use current time as seed
        util.setRandomSeed();
    }else if (string_command == "q"){
        return QUIT_GAME;
    }else if (string_command == "r"){
        return RESTART_GAME;
    }

    // select DLC

    // DLC selection
    util.requestInput<string>(in, out, string_command, 
        "Do you want to turn on DLC1 - random dungeon floor (y/n):", yes_no_validator);

    if(string_command == "y"){
        out << "DEFAULT: " << endl;
        out << "Maximum number of rooms: 6" << endl;
        out << "Minimum number of rooms: 5" << endl;
        out << "Maximum width of rooms: 25" << endl;
        out << "Minimum width of rooms: 12" << endl;
        out << "Maximum height of rooms: 11" << endl;
        out << "Minimum height of rooms: 7" << endl;

        int max_rooms = 6;
        int min_rooms = 5;
        int max_width = 25;
        int min_width = 12;
        int max_height = 11;
        int min_height = 7;

        util.requestInput<string>(in, out, string_command,
            "Do you want to customize the dungeon dimensions (y/n):", yes_no_validator);

        if(string_command == "y"){
            auto validator1 = [](int x){return x >= 4 && x <= 6;};
            util.requestInput<int>(in, out, max_rooms, "Input maximum number of rooms (4 - 6):", validator1);

            auto validator2 = [max_rooms](int x){return x >= 4 && x <= 6 && x <= max_rooms;};
            util.requestInput<int>(in, out, min_rooms, "Input minimum number of rooms (4 - 6):", validator2);

            auto validator3 = [](int x){return x >= 11 && x <= 25;};
            util.requestInput<int>(in, out, max_width, "Input maximum width of rooms (11 - 25):", validator3);

            auto validator4 = [max_width](int x){return x >= 9 && x <= 12 && x <= max_width;};
            util.requestInput<int>(in, out, min_width, "Input minimum width of rooms: (9 - 12):", validator4);

            auto validator5 = [](int x){return x >= 6 && x <= 11;};
            util.requestInput<int>(in, out, max_height, "Input maximum height of rooms (6 - 11):", validator5);

            auto validator6 = [max_height](int x){return x >= 6 && x <= 8 && x <= max_height;};
            util.requestInput<int>(in, out, min_height, "Input minimum height of rooms (6 - 8):", validator6);
        }

        FloorGenerator generator(max_rooms, min_rooms, max_width, min_width, max_height, min_height);
        randomlyGenerateFloors(generator);
    }else if (string_command == "n"){
        createFloorsFromFile(map_file);
    }else if (string_command == "q"){
        return QUIT_GAME;
    }else if (string_command == "r"){
        return RESTART_GAME;
    }

    // DLC 2 selection - Inventory DLC
    util.requestInput<string>(in, out, string_command, 
        "Do you want to turn on DLC2 - Inventory (y/n):", yes_no_validator);
    
    if(string_command == "y"){
        inventory_enabled = true;

        out << "New Commands:" << endl;
        out << "s [direction] - picks up and stores the potion in the inventory." << endl;
        out << "p - prints the inventory." << endl;
        out << "d [number] - drinks the [number]th potion in inventory." << endl; 
    }else if (string_command == "q"){
        return QUIT_GAME;
    }else if (string_command == "r"){
        return RESTART_GAME;
    }


    // DLC 3 selection - merchant DLC
    util.requestInput<string>(in, out, string_command, 
        "Do you want to turn on DLC3 - Trading With Merchants (y/n):", yes_no_validator);

    if(string_command == "y"){
        merchant_enabled = true;
        out << "New Commands:" << endl;
        out << "t [direction] - Send a trade request towards the merchant in [direction]. Merchant will print his storage if PC is not hostile to Merchants" << endl;
        out << "b [direction] [number] - Buy the [number]th potion from the merchant's storage in [direction]." << endl;
    }else if (string_command == "q"){
        return QUIT_GAME;
    }else if (string_command == "r"){
        return RESTART_GAME;
    }


    action_message += "Welcome to the dungeon! May good luck always be with you! ";

    // initialize the floor and the player 
    levels[current_level].initializeFloor(player);
    return CONTINUE_GAME;
};

int Controller::takeTurn(istream& in, ostream& out){
    /*
    Valid Command List
    • no,so,ea,we,ne,nw,se,sw: moves the player character one block in the appropriate cardinal direction.
    • u direction: uses the potion indicated by the direction (e.g. no, so, ea).
    • a direction: attacks the enemy in the specified direction
    • f: stops enemies from moving until this key is pressed again.
    • r: restarts the game. All stats, inventory, and gold are reset. A new race should be selected.
    • q: allows the player to admit defeat and exit the game.
    */

    // clears action message
    action_message = "";

    // reset enemy move status
    for(auto enemy : levels[current_level].getEnemies()){
        enemy->setMoved(false);
    }
    
    // Take player command
    string command;
    Util util;
    bool inventory_dlc_on = inventory_enabled;
    bool merchant_dlc_on = merchant_enabled;
    auto command_validator = [inventory_dlc_on, merchant_dlc_on](string c){return c == "no" || c == "so" 
        || c == "ea" || c == "we" || c == "ne" || c == "nw" || c == "se" || c == "sw" 
        || c == "u" || c == "a" || c == "f" || c == "r" || c == "q" 
        || (inventory_dlc_on && c == "s") || (inventory_dlc_on && c == "d") || (inventory_dlc_on && c == "p")
        || (merchant_dlc_on && c == "t") || (merchant_dlc_on && c == "b");};
    auto direction_validator = [](string c) {return c == "no" || c == "so" || c == "ea" || c == "we" 
        || c == "ne" || c == "nw" || c == "se" || c == "sw";};
    
    util.requestInput<string>(in, out, command, "Input a command:", command_validator);

    // Process player command
    if(command == "q"){
        return QUIT_GAME;
    }else if(command == "r"){
        return RESTART_GAME;
    }else if(command == "f"){
        Enemy::pause_enemy_move = !(Enemy::pause_enemy_move);
    }else if (command == "u" || command == "a" || (inventory_dlc_on && command == "s")
        || (merchant_dlc_on && command == "t")){
        // player uses potion or attacks (or store potion with inventory dlc)
        string command2;
        util.requestInput<string>(in, out, command2, "", direction_validator);
        player->takeTurn(levels[current_level], action_message, command, command2);
        if (merchant_dlc_on && command == "t"){
            return CONTINUE_GAME;
        }
        
    }else if((inventory_dlc_on && command == "d")){
        int command2;
        util.requestInput<int>(in, out, command2, "");

        if(command2 >= player->getInventorySize() || command2 < 0){
            action_message += "PC searched index " + std::to_string(command2) + " in his inventory and found nothing. ";
        }else{
            string str_command2 = std::to_string(command2);
            player->takeTurn(levels[current_level], action_message, command, str_command2);
        }
        
        
    }else if (merchant_dlc_on && command == "b"){
            string command2;
            util.requestInput<string>(in, out, command2, "", direction_validator);
            int command3;
            util.requestInput<int>(in, out, command3, "");
            Tile requested_tile = Tile::getTileInDirection(player->getTile(), command2);
            Enemy* enemy = levels[current_level].getEnemy(requested_tile);
            if (enemy == nullptr || enemy->getRace() != "Merchant"){
                action_message += "There is not a Merchant in the " + command2 + " direction and you bought nothing. ";
            }
            else{
                Merchant *merchant = dynamic_cast<Merchant*>(enemy);
                if (Merchant::is_hostile){
                    action_message += "The Merchant was hostile and you bought nothing. ";
                }
                else{
                    player->buyItem(merchant, command3, action_message);
                }
            }
            return CONTINUE_GAME;
    }
    else if(inventory_dlc_on && command == "p"){
        action_message += "PC opens his inventory and wants to see what he can make use of. \n";
        player->printInventory(action_message); // TODO add action messages
        return CONTINUE_GAME;
    }else{
        // player moves
        player->takeTurn(levels[current_level], action_message, command);
        
        // check if player is on stairs
        if(player->getPrevChar() == '\\'){
            ++current_level;

            // reset the player's prevchar to prevent the stairs from appearing below the PC on the next level
            player->setPrevChar('.');

            // check if the player has won the game
            if (current_level == MAX_LEVEL){
                int score = player->getScore();
                out << "You won the game! Your score is: " + std::to_string(score) + "! " << endl;
                return RESTART_GAME;
            }else{
                // initialize the next level
                action_message += "You moved to level " + std::to_string(current_level + 1) + ". ";
                levels[current_level].initializeFloor(player);
                player->detach();
                return CONTINUE_GAME;
            }
        }
    }

    vector<Enemy*> dead_enemies;
    vector<Gold*> dead_gold;

    // after player's turn, process dead enemies
    for(Enemy* enemy : levels[current_level].getEnemies()){
        if(enemy->isDead()){
            Tile tile = enemy->getTile();
            string race = enemy->getRace();

            dead_enemies.push_back(enemy);

            // process drops
            action_message += race + " is slained by PC. ";
            if(race == "Human"){
                // human drops gold
                dead_gold.push_back(new Gold("2 normal piles dropped by human", 4, true, true, tile));
            }else if(enemy->getRace() == "Merchant"){
                // merchant drops gold
                dead_gold.push_back(new Gold("Merchant Hoard", 4, true, true, tile));
            }else if(enemy->getRace() == "Dragon"){
                Gold* dragon_gold = static_cast<Dragon *>(enemy)->getGold();
                // dragon hoard is now pickable
                dragon_gold->setCanWalk(true);
                dragon_gold->setCanPick(true);
                // normal enemy drops gold
                int treasure = util.getRandomNumber(1, 2);
                player->pickGold(treasure);
                action_message += "PC loots " + std::to_string(treasure) + " gold. ";
            }else{
                // normal enemy drops gold
                int treasure = util.getRandomNumber(1, 2);
                player->pickGold(treasure);
                action_message += "PC loots " + std::to_string(treasure) + " gold. ";
            }
        }
    }

    // clear memory of dead enemies (also changes tile char to .)
    for(Enemy* enemy : dead_enemies){
        levels[current_level].removeEnemy(enemy->getTile());
        delete enemy;
        enemy = nullptr;
    }

    // spawn dead gold (also changes tile char to G)
    for(Gold* gold : dead_gold){
        levels[current_level].addGold(gold);
    }

    // after player's turn, process enemies' turn

    // Make enemies attack or move in a line-by-line fashion
    vector<vector<char>> grid = levels[current_level].getGrid();
    for(int y = 0 ; y < grid.size() ; ++y){
        for(int x = 0 ; x < grid[y].size() ; ++x){
            Enemy* enemy = levels[current_level].getEnemy(Tile(x, y));
            if(enemy == nullptr){
                continue;
            }
            enemy->takeTurn(levels[current_level], action_message, player);
        }
    }

    // check if player is dead
    if(player->isDead()){
        out << action_message << endl;
        out << "Unfortunately, you did not make it out of the dungeon alive as a " << player->getRace() << ". Your final score is: " << player->getScore() << endl;
        return RESTART_GAME;
    }

    return CONTINUE_GAME;
}

void Controller::createFloorsFromFile(string file_name){
    ifstream in{file_name};

    for(int maps_created = 0; maps_created < MAX_LEVEL; ++maps_created){
        int c = in.peek();

        // change to default map if the given file run out of content
        if(c == EOF){
            in = ifstream{DEFAULT_MAP_NAME};
        }

        // create the floor
        Floor floor{in};
        this->levels.push_back(floor);
    }
};

void Controller::randomlyGenerateFloors(FloorGenerator& generator){

    for(int i = 0; i < MAX_LEVEL; ++i){
        // get an empty floor
        Floor floor{};

        // generate the floor
        generator.generateRandomFloor(floor);
        this->levels.push_back(floor);
    }

};


// reset the game
void Controller::resetGame(){
    levels.clear();
    current_level = 0;
    Merchant::is_hostile = false;
    Enemy::pause_enemy_move = false;
    inventory_enabled = false;
    merchant_enabled = false;
    action_message = "";
    Potion::rh_learned = false;
    Potion::ph_learned = false;
    Potion::ba_learned = false;
    Potion::bd_learned = false;
    Potion::wa_learned = false;
    Potion::wd_learned = false;
    // The player pointer will be reset in initializeGame

    delete player;
    player = nullptr;
}

void Controller::printGameState(ostream& out){
    levels[current_level].printFloor(out);

    // print player status
    out << "Race: " << player->getRace() << "\t" << "Gold: " << player->getGold() << 
        "\t\t" << "Floor " << current_level + 1 << endl;

    out << "HP:" << player->getHp() << endl;
    out << "ATK:" << player->getAtk() << endl;
    out << "DEF:" << player->getDef() << endl;

    out << "Action: " << action_message << endl;
}

// destructor
Controller::~Controller(){
    delete player;
    player = nullptr;
}
