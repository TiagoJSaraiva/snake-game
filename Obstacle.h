#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "Position.h"

class Obstacle {
public:
    Position position;

    Obstacle(int x, int y);
    void draw() const;
};

#endif // OBSTACLE_H
