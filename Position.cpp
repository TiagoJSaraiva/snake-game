#include "Position.h"

bool Position::operator==(const Position& other) const {
    return x == other.x && y == other.y;
}
