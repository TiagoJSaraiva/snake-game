#ifndef POISONFOOD_H
#define POISONFOOD_H

#include "Position.h"
#include "Snake.h" // Inclui a classe Snake para usar no método spawn

class PoisonFood {
public:
    Position position;

    PoisonFood();
    void spawn(const Snake& snake, const std::vector<Position>& obstacles, const Position& normalFood);
    void draw() const;
};

#endif // POISONFOOD_H
