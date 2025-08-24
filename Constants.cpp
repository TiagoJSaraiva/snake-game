#include "Constants.h"

std::mt19937 rng(static_cast<unsigned int>(time(nullptr)));
std::uniform_int_distribution<int> distX(0, GRID_WIDTH - 1);
std::uniform_int_distribution<int> distY(0, GRID_HEIGHT - 1);
