#include "Obstacle.h"
#include "raylib.h"
#include "Constants.h"

Obstacle::Obstacle(int x, int y) : position(x, y) {}

void Obstacle::draw() const {
    DrawRectangle(
        position.x * GRID_SIZE,
        position.y * GRID_SIZE,
        GRID_SIZE - 1,
        GRID_SIZE - 1,
        BROWN
    );
}
