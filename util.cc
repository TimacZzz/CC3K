#include "util.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;

// set random seed for the program, default seed is the current time
void Util::setRandomSeed(int seed){
    srand(seed);
}

// generate random number between min and max
int Util::getRandomNumber(int min, int max){
    return rand() % (max - min + 1) + min;
}
