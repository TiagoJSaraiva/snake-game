#ifndef FOOD_H
#define FOOD_H

#include <vector>
#include "Position.h"
#include "Snake.h" // Inclui a classe Snake para poder usar no método spawn

class Food {
public:
    Position position;
    bool active;

    Food();

    void spawn(const Snake& snake, const std::vector<Position>& obstacles, const std::vector<Position>& poisonFood);
    void draw() const;
};

#endif // FOOD_H
