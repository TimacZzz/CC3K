#include "dwarf.h"

Dwarf::Dwarf(Tile tile): Enemy{100, 100, 20, 20, 30, 30, 0, "Dwarf", tile}{
    symbol = 'W';
}
