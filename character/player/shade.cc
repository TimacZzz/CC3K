#include "shade.h"

Shade::Shade(Tile tile): Player{125, 125, 25, 25, 25, 25, 0, "Shade", tile}{
    prevchar = '.';
}

// Special method for Shade to have 1.5 times of the score
int Shade::getScore(){
    return score * 1.5;
}
