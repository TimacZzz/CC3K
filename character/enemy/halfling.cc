#include "halfling.h"

Halfling::Halfling(Tile tile): Enemy{100, 100, 15, 15, 20, 20, 0, "Halfling", tile}{
    symbol = 'L';
}
