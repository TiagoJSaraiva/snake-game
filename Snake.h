#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include "Position.h"

class Snake {
public:
    std::vector<Position> body;
    Position direction;
    bool alive;

    Snake();

    void reset();
    void move();
    void grow();
    bool checkWallCollision() const;
    bool checkSelfCollision() const;
    void setDirection(const Position& newDirection);
    void draw() const;
};

#endif // SNAKE_H
