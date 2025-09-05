#ifndef GAME_H
#define GAME_H

#include "snake.h"
#include "frame.h"

class snakeGame : public frame {
    public:
    snakeGame();    // calls frame constructor in multi-threaded mode    
    
    void run();
    void pauseGame();
    void endGame();
    // void reset();

    private:
    snake sneakySnake;      // snake object
    s_pos foodPos;          // food position

    // Food
    bool foodConsumed();
    void placeFood();

    // Collision checks
    bool checkCollision();
    bool checkBorderCollision();

    // Update object positions on frame
    void updateSnake(char cmd);
    void updateFrameLayout();

    protected:
};

#endif