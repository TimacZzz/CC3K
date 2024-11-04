#include <iostream>
#include "controller.h"

using namespace std;

const string DEFAULT_MAP_NAME = "emptyfloor.txt";

const int RESTART_GAME = 1;
const int QUIT_GAME = 2;
const int CONTINUE_GAME = 3;

int main(int argc, char* argv[]) {
    // create map from file if there's one
    string map_file;
    if(argc > 1){
        map_file = argv[1];
    }else{
        map_file = DEFAULT_MAP_NAME;
    }

    Controller controller;
    int result = controller.initializeGame(cin, cout, map_file);

    while(true){
        try{
            if(result == QUIT_GAME){
                break;
            }else if(result == RESTART_GAME){
                controller.resetGame();
                result = controller.initializeGame(cin, cout, map_file);
                continue;
            }else if(result == CONTINUE_GAME){
                controller.printGameState(cout);
            }
            result = controller.takeTurn(cin, cout);
        }catch(...){
            cin.clear();
            controller.resetGame();
            result = controller.initializeGame(cin, cout, map_file);
        }
    }

}
