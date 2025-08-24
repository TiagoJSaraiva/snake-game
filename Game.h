#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include <vector>
#include <random>

// Inclui os cabeçalhos das outras classes
#include "Snake.h"
#include "Food.h"
#include "Obstacle.h"
#include "PoisonFood.h"
#include "Constants.h"

// Enumerações para o estado do jogo e a dificuldade
enum class GameState { MENU, DIFFICULTY_SELECT, PLAYING, GAME_OVER };
enum class Difficulty { EASY, MEDIUM, HARD };

class Game {
private:
    Snake snake;
    Food food;
    GameState gameState;
    Difficulty currentDifficulty;
    std::vector<Obstacle> obstacles;
    std::vector<PoisonFood> poisonFoods;
    int score;
    float gameTimer;
    float moveTimer;
    const float MOVE_SPEED = 0.15f;
    Font mainFont;

    // Métodos privados
    void initGame();
    void generateObstacles();
    std::vector<Position> getObstaclePositions() const;
    std::vector<Position> getPoisonPositions() const;
    void updateGameplay();
    void handleInput();

public:
    Game();
    void run();
    void update();
    void draw() const;
};

#endif // GAME_H
