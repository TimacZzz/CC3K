#include "drow.h"

Drow::Drow(Tile tile): Player{150, 150, 25, 25, 15, 15, 0, "Drow", tile}{
    prevchar = '.';
    score = 0;
}
