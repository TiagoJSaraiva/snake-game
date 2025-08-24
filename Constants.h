#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <random>
#include <ctime>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int GRID_SIZE = 20;
const int GRID_WIDTH = SCREEN_WIDTH / GRID_SIZE;
const int GRID_HEIGHT = SCREEN_HEIGHT / GRID_SIZE;
const int MAX_SNAKE_LENGTH = 100;

// Gerador de números aleatórios
extern std::mt19937 rng;
extern std::uniform_int_distribution<int> distX;
extern std::uniform_int_distribution<int> distY;

#endif // CONSTANTS_H
