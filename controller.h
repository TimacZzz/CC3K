#ifndef CONTROLLER
#define CONTROLLER

#include <iostream>
#include <vector>
#include <fstream> 
#include <memory>
#include <string>
#include "floor/floor.h"

using namespace std;

class FloorGenerator;

class Controller{
    private:
        const int MAX_LEVEL = 5;
        const string DEFAULT_MAP_NAME = "emptyfloor.txt";
        vector<Floor> levels;
        int current_level = 0;
        Player* player = nullptr;
        bool inventory_enabled = false;
        bool merchant_enabled = false;
        string action_message = "";

        void createFloorsFromFile(string file_name);
        void randomlyGenerateFloors(FloorGenerator& generator);

    public:
        int initializeGame(istream& in, ostream& out, const string& map_file);
        int takeTurn(istream& in, ostream& out);
        void resetGame();
        void printGameState(ostream& out);
        ~Controller();
};

#endif
