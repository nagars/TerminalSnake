// Declares snake class

#include "snake.h"
#include "string.h"

snake::snake(void){

    // Reset position of snake head
    memset(&headPos,0,sizeof(s_pos));

    // Set design of snake head to be printed
    design = '^';
}

snake::snake(char c): design(c){

    // Reset position of snake head
    memset(&headPos,0,sizeof(s_pos));

}

snake::snake(s_pos position): headPos(position){

    // Set design of snake head to be printed
    design = '^';

}

snake::snake(s_pos position, char c): headPos(position), design(c){

}

void snake::setHeadPos(const s_pos pos){

    headPos = pos;
}
    
s_pos snake::getHeadPos(void){

    return headPos;
}
    
void snake::setdesign(const char c){

    design = c;
}

void snake::extendSnake(void){

    // Append to snakeBodyElement vector
    // tracking size of the snake in terms of individual
    // elements

    char c;
    // Based on direction of snake, set what character to be used
    // to define body element
    if((design == SNAKE_HEAD_WEST) || (design == SNAKE_HEAD_EAST)){
        c = SNAKE_BODY_HORIZ;
    }else if((design == SNAKE_HEAD_NORTH)||(design == SNAKE_HEAD_SOUTH)){
        c = SNAKE_BODY_VERT;
    }

    // Append body Element to snake body vector
    body.emplace_back(snakeBodyElement(headPos, c));
}

snakeBodyElement::snakeBodyElement(s_pos position, char design): pos(position), design(design){

}

