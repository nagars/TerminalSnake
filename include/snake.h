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

// Tracks direction of the snake
typedef enum {
    DIR_NORTH,
    DIR_SOUTH,
    DIR_EAST,
    DIR_WEST
}e_DIR;

// Each body block of the snake
class snakeBodyElement {
    public:
    snakeBodyElement(s_pos position);
    snakeBodyElement(s_pos position, char design);

    // Setters / Getters
    void setPos(s_pos position);
    s_pos getPos(void);
    void setDesign(char c);
    char getDesign(void);

    private:
    s_pos pos;                          // position in frame
    char design = SNAKE_BODY_VERT;      // What to print

    protected:
};

// Snake object (Head + Body)
class snake {
    public:
    snake();
    snake(e_DIR dir);                   // Sets design to print and direction
    snake(s_pos position);              // Sets position of head
    snake(s_pos position, e_DIR dir);   // Sets both design and position of head

    // Setters / Getters
    void setHeadPos(const s_pos pos);
    s_pos getHeadPos(void);
    void setDirection(e_DIR);
    e_DIR getDirection(void);

    char getDesign(void);
    bool getCollsionStatus(void);       // True if head collides with body. Refreshed on each move.
    const std::vector<snakeBodyElement>& getBody (void);    // Return a constant reference to the body
                                                            // of the snake

    void extendSnake(void);
    void moveSnake(void);
    void checkCollision(void);

    private:
    s_pos headPos;                      // Position in terminal based on row and col
    char design = SNAKE_HEAD_NORTH;     // Character to print. Used to track direction
    e_DIR direction = DIR_NORTH;        // Tracks direction of movement
    std::vector<snakeBodyElement> body; // Vector of body elements
    bool f_collisionOccurred = false;   // flag set when collision with body

    void setDesign(e_DIR);      // Adjust the design based on direction
    void moveHead(void);

    protected:
};

#endif