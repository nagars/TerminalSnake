#ifndef GAME_H
#define GAME_H

#include "snake.h"
#include "frame.h"

class snakeGame : public frame {
    public:
    snakeGame();    // setup signal trap (SIGWINCH) and user input

    private:
    void run();
    void pause();
    void exit();

    snake sneakySnake;            // snake object
    s_pos foodPos;          // food position
    uint16_t sleepTime;     // time between each game loop

    protected:
};

#endif