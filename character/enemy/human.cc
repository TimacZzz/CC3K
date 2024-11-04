#include "human.h"

Human::Human(Tile tile): Enemy{140, 140, 20, 20, 20, 20, 2, "Human", tile}{
    symbol = 'H';
}
