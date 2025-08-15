#ifndef SNAKE_H
#define SNAKE_H

#include <stdint.h>
#include <vector>


#define SNAKE_HEAD_EAST     '>'
#define SNAKE_HEAD_WEST     '<'
#define SNAKE_HEAD_NORTH    '^'
#define SNAKE_HEAD_SOUTH    'v'
#define SNAKE_BODY_VERT     '|'
#define SNAKE_BODY_HORIZ    '-'
#define SNAKE_FOOD          '*'

typedef struct {
    uint16_t x;
    uint16_t y;
}s_pos;

class snakeBodyElement {
    public:
    snakeBodyElement(s_pos position, char design);

    void setPos(s_pos pos);
    s_pos getPos(void);
    void setdesign(char design);

    private:
    s_pos pos;      // position in frame
    char design;    // What to print

    protected:
};

class snake {
    public:
    snake();
    snake(char c);          // Sets design
    snake(s_pos position);  // Sets position of head
    snake(s_pos position, char c);  // Sets both design and position of head

    void setHeadPos(const s_pos pos);
    s_pos getHeadPos(void);
    void setdesign(const char c);
    void extendSnake(void);

    private:
    s_pos headPos;      // Position in terminal based on row and col
    char design;        // Character to print
    std::vector<snakeBodyElement> body;

    protected:
};

#endif