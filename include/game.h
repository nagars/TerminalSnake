#ifndef GAME_H
#define GAME_H

#include "snake.h"
#include "frame.h"

class snakeGame : public frame {
    public:
    snakeGame();    // setup signal trap (SIGWINCH) and user input
    
    void run();
    // void pause();
    void endGame();
    // void reset();

    private:
    snake sneakySnake;      // snake object
    s_pos foodPos;          // food position
    uint16_t sleepTime;     // time between each game loop

    bool foodConsumed();
    void placeFood();
    bool checkCollision();
    bool checkBorderCollision();
    void updateSnake(char cmd);
    void updateFrameLayout();

    protected:
};

#endif