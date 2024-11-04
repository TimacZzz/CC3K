#include "vampire.h"
#include <cmath>
#include "../../util.h"
#include "../enemy/merchant.h"

Vampire::Vampire(Tile tile): Player{50, 50, 25, 25, 25, 25, 0, "Vampire", tile}{
    prevchar = '.';
    score = 0;
}
