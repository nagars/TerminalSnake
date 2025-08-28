#ifndef SNAKE_H
#define SNAKE_H

#include <stdint.h>
#include <vector>
#include "common.h"

// Characters to be printed to terminal
#define SNAKE_HEAD_EAST     '>'
#define SNAKE_HEAD_WEST     '<'
#define SNAKE_HEAD_NORTH    '^'
#define SNAKE_HEAD_SOUTH    'v'
#define SNAKE_BODY_VERT     '|'
#define SNAKE_BODY_HORIZ    '-'
#define SNAKE_FOOD          '*'

typedef enum {
    DIR_NORTH,
    DIR_SOUTH,
    DIR_EAST,
    DIR_WEST
}e_DIR;

class snakeBodyElement {
    public:
    snakeBodyElement(s_pos position, char design);

    void setPos(s_pos position);
    s_pos getPos(void);
    void setDesign(char c);
    char getDesign(void);


    private:
    s_pos pos;      // position in frame
    char design = '|';    // What to print

    protected:
};

class snake {
    public:
    snake();
    snake(e_DIR dir);          // Sets design
    snake(s_pos position);  // Sets position of head
    snake(s_pos position, e_DIR dir);  // Sets both design and position of head

    void setHeadPos(const s_pos pos);
    s_pos getHeadPos(void);
    void setDirection(e_DIR);
    e_DIR getDirection(void);
    char getDesign(void);
    bool getCollsionStatus(void);       // True if head collides with body. Refreshed on each move.

    void extendSnake(void);
    void moveSnake(void);
    void checkCollision(void);

    const std::vector<snakeBodyElement>& getBody (void);    // Return a constant reference to the body
                                                            // of the snake

    private:
    s_pos headPos;              // Position in terminal based on row and col
    char design = '^';          // Character to print. Used to track direction
    e_DIR direction = DIR_NORTH;
    std::vector<snakeBodyElement> body;

    bool f_collisionOccurred = false;

    void setDesign(e_DIR);
    void moveHead(void);

    protected:
};

#endif