#include "PoisonFood.h"
#include "raylib.h"
#include "Constants.h"

PoisonFood::PoisonFood() {}

void PoisonFood::spawn(const Snake& snake, const std::vector<Position>& obstacles, const Position& normalFood) {
    bool validPosition = false;
    const Position& head = snake.body[0];
    int attempts = 0;

    while (!validPosition && attempts < 1000) {
        attempts++;
        position.x = distX(rng);
        position.y = distY(rng);

        validPosition = true;
        int distanceToHead = abs(position.x - head.x) + abs(position.y - head.y);
        if (distanceToHead < 3) validPosition = false;
        for (const auto& segment : snake.body) if (position == segment) { validPosition = false; break; }
        for (const auto& obstacle : obstacles) if (position == obstacle) { validPosition = false; break; }
        if (normalFood.x != -1 && position == normalFood) validPosition = false;
    }
}

void PoisonFood::draw() const {
    DrawRectangle(
        position.x * GRID_SIZE,
        position.y * GRID_SIZE,
        GRID_SIZE - 1,
        GRID_SIZE - 1,
        PURPLE
    );
}
