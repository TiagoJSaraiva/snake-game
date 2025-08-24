#include "Snake.h"
#include "raylib.h"
#include "Constants.h"

Snake::Snake() {
    reset();
}

void Snake::reset() {
    body.clear();
    body.push_back(Position(GRID_WIDTH/2, GRID_HEIGHT/2));
    body.push_back(Position(GRID_WIDTH/2 - 1, GRID_HEIGHT/2));
    body.push_back(Position(GRID_WIDTH/2 - 2, GRID_HEIGHT/2));
    direction = Position(1, 0);
    alive = true;
}

void Snake::move() {
    for (int i = body.size() - 1; i > 0; i--) {
        body[i] = body[i - 1];
    }
    body[0].x += direction.x;
    body[0].y += direction.y;
}

void Snake::grow() {
    if (body.size() < MAX_SNAKE_LENGTH) {
        body.push_back(body.back());
    }
}

bool Snake::checkWallCollision() const {
    const Position& head = body[0];
    return (head.x < 0 || head.x >= GRID_WIDTH ||
            head.y < 0 || head.y >= GRID_HEIGHT);
}

bool Snake::checkSelfCollision() const {
    const Position& head = body[0];
    for (size_t i = 1; i < body.size(); i++) {
        if (head == body[i]) {
            return true;
        }
    }
    return false;
}

void Snake::setDirection(const Position& newDirection) {
    if (!(direction.x == -newDirection.x && direction.y == -newDirection.y)) {
        direction = newDirection;
    }
}

void Snake::draw() const {
    for (size_t i = 0; i < body.size(); i++) {
        Color color = (i == 0) ? LIME : GREEN;
        DrawRectangle(
            body[i].x * GRID_SIZE,
            body[i].y * GRID_SIZE,
            GRID_SIZE - 1,
            GRID_SIZE - 1,
            color
        );
    }
}
