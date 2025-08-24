#include "Game.h"
#include <iostream>

Game::Game() : gameState(GameState::MENU), currentDifficulty(Difficulty::EASY), score(0), gameTimer(0.0f), moveTimer(0.0f) {}

void Game::initGame() {
    snake.reset();
    obstacles.clear();
    poisonFoods.clear();

    if (currentDifficulty == Difficulty::MEDIUM || currentDifficulty == Difficulty::HARD) generateObstacles();
    if (currentDifficulty == Difficulty::HARD) {
        poisonFoods.resize(10);
    }
    food.spawn(snake, getObstaclePositions(), getPoisonPositions());
    if (currentDifficulty == Difficulty::HARD) {
        for (auto& poison : poisonFoods) poison.spawn(snake, getObstaclePositions(), food.position);
    }

    score = 0;
    gameTimer = 0.0f;
    moveTimer = 0.0f;
}

void Game::generateObstacles() {
    obstacles.clear();
    const int TOTAL_OBSTACLE_BLOCKS = 60;
    const int MIN_CLUSTER_SIZE = 4;
    const int MAX_CLUSTER_SIZE = 10;

    std::vector<Position> directions = {Position(0, 1), Position(0, -1), Position(1, 0), Position(-1, 0)};

    while (obstacles.size() < TOTAL_OBSTACLE_BLOCKS) {
        int clusterSize = std::uniform_int_distribution<int>(MIN_CLUSTER_SIZE, MAX_CLUSTER_SIZE)(rng);

        std::vector<Position> currentCluster;
        bool clusterStarted = false;
        int attempts = 0;

        while (!clusterStarted && attempts < 100) {
            attempts++;
            Position seed(distX(rng), distY(rng));
            bool isTooCloseToSnake = (abs(seed.x - GRID_WIDTH / 2) < 4 && abs(seed.y - GRID_HEIGHT / 2) < 4);
            bool isOverlappingExisting = false;
            for (const auto& obs : obstacles) {
                if (seed == obs.position) {
                    isOverlappingExisting = true;
                    break;
                }
            }

            if (!isTooCloseToSnake && !isOverlappingExisting) {
                currentCluster.push_back(seed);
                clusterStarted = true;
            }
        }

        if (!clusterStarted) continue;

        attempts = 0;
        while (currentCluster.size() < clusterSize && attempts < 1000) {
            attempts++;
            Position expandFrom = currentCluster[std::uniform_int_distribution<int>(0, currentCluster.size() - 1)(rng)];
            Position newPos = expandFrom;

            Position dir = directions[std::uniform_int_distribution<int>(0, directions.size() - 1)(rng)];
            newPos.x += dir.x;
            newPos.y += dir.y;

            bool is_valid = true;

            if (newPos.x < 0 || newPos.x >= GRID_WIDTH || newPos.y < 0 || newPos.y >= GRID_HEIGHT) {
                is_valid = false;
            }

            for (const auto& pos : currentCluster) {
                if (newPos == pos) {
                    is_valid = false;
                    break;
                }
            }
            for (const auto& obs : obstacles) {
                if (newPos == obs.position) {
                    is_valid = false;
                    break;
                }
            }

            if (is_valid) {
                currentCluster.push_back(newPos);
            }
        }

        for (const auto& pos : currentCluster) {
            obstacles.push_back(Obstacle(pos.x, pos.y));
        }
    }
}

std::vector<Position> Game::getObstaclePositions() const {
    std::vector<Position> positions;
    for (const auto& obstacle : obstacles) positions.push_back(obstacle.position);
    return positions;
}

std::vector<Position> Game::getPoisonPositions() const {
    std::vector<Position> positions;
    for (const auto& poison : poisonFoods) positions.push_back(poison.position);
    return positions;
}

void Game::update() {
    gameTimer += GetFrameTime();
    switch (gameState) {
        case GameState::MENU:
            if (IsKeyPressed(KEY_SPACE)) gameState = GameState::DIFFICULTY_SELECT;
            break;
        case GameState::DIFFICULTY_SELECT:
            if (IsKeyPressed(KEY_ONE)) { currentDifficulty = Difficulty::EASY; gameState = GameState::PLAYING; initGame(); }
            else if (IsKeyPressed(KEY_TWO)) { currentDifficulty = Difficulty::MEDIUM; gameState = GameState::PLAYING; initGame(); }
            else if (IsKeyPressed(KEY_THREE)) { currentDifficulty = Difficulty::HARD; gameState = GameState::PLAYING; initGame(); }
            else if (IsKeyPressed(KEY_ESCAPE)) gameState = GameState::MENU;
            break;
        case GameState::PLAYING:
            handleInput();
            updateGameplay();
            break;
        case GameState::GAME_OVER:
            if (IsKeyPressed(KEY_SPACE)) gameState = GameState::DIFFICULTY_SELECT;
            else if (IsKeyPressed(KEY_ESCAPE)) gameState = GameState::MENU;
            break;
    }
}

void Game::handleInput() {
    if (IsKeyPressed(KEY_UP)) snake.setDirection(Position(0, -1));
    else if (IsKeyPressed(KEY_DOWN)) snake.setDirection(Position(0, 1));
    else if (IsKeyPressed(KEY_LEFT)) snake.setDirection(Position(-1, 0));
    else if (IsKeyPressed(KEY_RIGHT)) snake.setDirection(Position(1, 0));
}

void Game::updateGameplay() {
    moveTimer += GetFrameTime();
    if (moveTimer >= MOVE_SPEED) {
        moveTimer = 0.0f;
        snake.move();
        if (snake.checkWallCollision() || snake.checkSelfCollision()) { snake.alive = false; gameState = GameState::GAME_OVER; return; }
        for (const auto& obstacle : obstacles) if (snake.body[0] == obstacle.position) { snake.alive = false; gameState = GameState::GAME_OVER; return; }
        for (const auto& poison : poisonFoods) if (snake.body[0] == poison.position) { snake.alive = false; gameState = GameState::GAME_OVER; return; }
        if (snake.body[0] == food.position) {
            score += 10; snake.grow();
            food.spawn(snake, getObstaclePositions(), getPoisonPositions());
            if (currentDifficulty == Difficulty::HARD) {
                for (auto& poison : poisonFoods) poison.spawn(snake, getObstaclePositions(), food.position);
            }
        }
    }
}

void Game::draw() const {
    switch (gameState) {
        case GameState::MENU: {
            const char* titleText = "SNAKE GAME";
            Vector2 titleSize = MeasureTextEx(mainFont, titleText, 80, 1);
            DrawTextEx(mainFont, titleText, {SCREEN_WIDTH / 2 - titleSize.x / 2, SCREEN_HEIGHT / 2 - 100.0f}, 80, 1, GREEN);

            const char* startText = "Press SPACE to start";
            Vector2 startSize = MeasureTextEx(mainFont, startText, 40, 1);
            DrawTextEx(mainFont, startText, {SCREEN_WIDTH / 2 - startSize.x / 2, SCREEN_HEIGHT / 2 - 20.0f}, 40, 1, WHITE);

            const char* moveText = "Use the arrow keys to move";
            Vector2 moveSize = MeasureTextEx(mainFont, moveText, 40, 1);
            DrawTextEx(mainFont, moveText, {SCREEN_WIDTH / 2 - moveSize.x / 2, SCREEN_HEIGHT / 2 + 20.0f}, 40, 1, GRAY);
            break;
        }
        case GameState::DIFFICULTY_SELECT: {
            const char* selectText = "SELECT DIFFICULTY";
            Vector2 selectSize = MeasureTextEx(mainFont, selectText, 60, 1);
            DrawTextEx(mainFont, selectText, {SCREEN_WIDTH / 2 - selectSize.x / 2, SCREEN_HEIGHT / 2 - 120.0f}, 60, 1, YELLOW);

            DrawTextEx(mainFont, "1 - EASY (Classic Snake)", {SCREEN_WIDTH / 2 - 140.0f, SCREEN_HEIGHT / 2 - 60.0f}, 40, 1, WHITE);
            DrawTextEx(mainFont, "2 - MEDIUM (With obstacles)", {SCREEN_WIDTH / 2 - 140.0f, SCREEN_HEIGHT / 2 - 30.0f}, 40, 1, WHITE);
            DrawTextEx(mainFont, "3 - HARD (Obstacles + rotten food)", {SCREEN_WIDTH / 2 - 180.0f, SCREEN_HEIGHT / 2.0f}, 40, 1, WHITE);

            const char* backText = "ESC - Back to menu";
            Vector2 backSize = MeasureTextEx(mainFont, backText, 36, 1);
            DrawTextEx(mainFont, backText, {SCREEN_WIDTH / 2 - backSize.x / 2, SCREEN_HEIGHT / 2 + 60.0f}, 36, 1, GRAY);

            DrawTextEx(mainFont, "Legend:", {50.0f, SCREEN_HEIGHT - 120.0f}, 28, 1, WHITE);
            DrawRectangle(50, SCREEN_HEIGHT - 95, 15, 15, RED);
            DrawTextEx(mainFont, "Normal food", {75.0f, SCREEN_HEIGHT - 95.0f}, 28, 1, WHITE);
            DrawRectangle(50, SCREEN_HEIGHT - 75, 15, 15, BROWN);
            DrawTextEx(mainFont, "Obstacles", {75.0f, SCREEN_HEIGHT - 75.0f}, 28, 1, WHITE);
            DrawRectangle(50, SCREEN_HEIGHT - 55, 15, 15, PURPLE);
            DrawTextEx(mainFont, "Rotten food (kills)", {75.0f, SCREEN_HEIGHT - 55.0f}, 28, 1, WHITE);
            break;
        }
        case GameState::PLAYING:
        {
            for (const auto& obstacle : obstacles) obstacle.draw();
            for (const auto& poison : poisonFoods) poison.draw();
            snake.draw();
            food.draw();
            DrawTextEx(mainFont, TextFormat("Score: %d", score), {10.0f, 10.0f}, 40, 1, WHITE);
            const char* difficultyText = (currentDifficulty == Difficulty::EASY) ? "EASY" :
                                         (currentDifficulty == Difficulty::MEDIUM) ? "MEDIUM" : "HARD";
            DrawTextEx(mainFont, TextFormat("Difficulty: %s", difficultyText), {10.0f, 40.0f}, 32, 1, GRAY);
            break;
        }
        case GameState::GAME_OVER: {
            const char* gameOverText = "GAME OVER";
            Vector2 gameOverSize = MeasureTextEx(mainFont, gameOverText, 80, 1);
            DrawTextEx(mainFont, gameOverText, {SCREEN_WIDTH / 2 - gameOverSize.x / 2, SCREEN_HEIGHT / 2 - 100.0f}, 80, 1, RED);

            const char* finalScoreText = TextFormat("Final Score: %d", score);
            Vector2 finalScoreSize = MeasureTextEx(mainFont, finalScoreText, 40, 1);
            DrawTextEx(mainFont, finalScoreText, {SCREEN_WIDTH / 2 - finalScoreSize.x / 2, SCREEN_HEIGHT / 2 - 40.0f}, 40, 1, WHITE);

            const char* diffText = (currentDifficulty == Difficulty::EASY) ? "EASY" :
                                   (currentDifficulty == Difficulty::MEDIUM) ? "MEDIUM" : "HARD";
            const char* difficultyLabelText = TextFormat("Difficulty: %s", diffText);
            Vector2 difficultyLabelSize = MeasureTextEx(mainFont, difficultyLabelText, 32, 1);
            DrawTextEx(mainFont, difficultyLabelText, {SCREEN_WIDTH / 2 - difficultyLabelSize.x / 2, SCREEN_HEIGHT / 2 - 10.0f}, 32, 1, GRAY);

            const char* selectDiffText = "SPACE - Select difficulty";
            Vector2 selectDiffSize = MeasureTextEx(mainFont, selectDiffText, 36, 1);
            DrawTextEx(mainFont, selectDiffText, {SCREEN_WIDTH / 2 - selectDiffSize.x / 2, SCREEN_HEIGHT / 2 + 30.0f}, 36, 1, GRAY);

            const char* menuText = "ESC - Main menu";
            Vector2 menuSize = MeasureTextEx(mainFont, menuText, 36, 1);
            DrawTextEx(mainFont, menuText, {SCREEN_WIDTH / 2 - menuSize.x / 2, SCREEN_HEIGHT / 2 + 60.0f}, 36, 1, GRAY);
            break;
        }
    }
}

void Game::run() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Snake Game - Raylib C++");
    SetTargetFPS(60);

    int fontCharacters[] = {
        ' ', '!', '"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/',
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ':', ';', '<', '=', '>', '?',
        '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
        'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '[', '\\', ']', '^', '_',
        '`', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
        'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '{', '|', '}', '~',
        'ç', 'Ç', 'ã', 'Ã', 'é', 'É', 'í', 'Í', 'ú', 'Ú', 'ó', 'Ó', 'á', 'Á', 'à', 'À',
        'ê', 'Ê', 'ô', 'Ô', 'õ', 'Õ', 'ü', 'Ü'
    };
    mainFont = LoadFontEx("Teko-SemiBold.ttf", 20, fontCharacters, sizeof(fontCharacters) / sizeof(int));

    while (!WindowShouldClose()) {
        update();
        BeginDrawing();
        ClearBackground(BLACK);
        draw();
        EndDrawing();
    }

    UnloadFont(mainFont);
    CloseWindow();
}
