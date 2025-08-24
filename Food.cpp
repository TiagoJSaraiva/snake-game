#include "Food.h"
#include "raylib.h"
#include "Constants.h"

Food::Food() : active(false) {}

void Food::spawn(const Snake& snake, const std::vector<Position>& obstacles, const std::vector<Position>& poisonFood) {
    bool validPosition = false;
    int attempts = 0;

    while (!validPosition && attempts < 1000) {
        attempts++;
        position.x = distX(rng);
        position.y = distY(rng);

        validPosition = true;
        for (const auto& segment : snake.body) if (position == segment) { validPosition = false; break; }
        for (const auto& obstacle : obstacles) if (position == obstacle) { validPosition = false; break; }
        for (const auto& poison : poisonFood) if (position == poison) { validPosition = false; break; }
    }
    active = validPosition;
}

void Food::draw() const {
    if (active) {
        DrawRectangle(
            position.x * GRID_SIZE,
            position.y * GRID_SIZE,
            GRID_SIZE - 1,
            GRID_SIZE - 1,
            RED
        );
    }
}
