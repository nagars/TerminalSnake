// Declares snake class

#include "snake.h"
#include "string.h"
#include "common.h"

snake::snake(void){

    // Reset position of snake head
    memset(&headPos,0,sizeof(s_pos));
}

snake::snake(e_DIR dir): direction(dir){

    // Reset position of snake head
    memset(&headPos,0,sizeof(s_pos));

    // Set the design of the snake head
    setDesign(dir);

}

snake::snake(s_pos position): headPos(position){
}

snake::snake(s_pos position, e_DIR dir): headPos(position), direction(dir){

    // Set the design of the snake head
    setDesign(dir);
}

void snake::setDesign(e_DIR dir){

    switch(dir){
        case DIR_NORTH:
        design = SNAKE_HEAD_NORTH;
        break;
        case DIR_SOUTH:
        design = SNAKE_HEAD_SOUTH;
        break;
        case DIR_EAST:
        design = SNAKE_HEAD_EAST;
        break;
        case DIR_WEST:
        design = SNAKE_HEAD_WEST;
        break;
        default:
        break;
    }

}

void snake::setHeadPos(const s_pos pos){
    headPos = pos;
}
    
s_pos snake::getHeadPos(void){
    return headPos;
}
    
void snake::setDirection(e_DIR dir){
    direction = dir;
}

e_DIR snake::getDirection(void){
    return direction;
}

void snake::extendSnake(void){

    // Append to snakeBodyElement vector
    // tracking size of the snake in terms of individual
    // elements

    char c;
    // Based on direction of snake, set what character to be used
    // to define body element
    if((direction == DIR_EAST) || (direction == DIR_WEST)){
        c = SNAKE_BODY_HORIZ;
    }else if((direction == DIR_NORTH)||(direction == DIR_SOUTH)){
        c = SNAKE_BODY_VERT;
    }

    // Append body Element to snake body vector
    body.emplace_back(snakeBodyElement(headPos, c));

    // Move head based on direction
    moveHead();

}

void snake::moveSnake(void){

    if(body.size() > 0){
        // Move first body element to head position
        body[0].setPos(headPos);

        // Update the direction of the lead body element
        // based on direction of the head
        switch(direction){
            case DIR_NORTH:
            case DIR_SOUTH:
            body[0].setDesign(SNAKE_BODY_VERT);
            break;

            case DIR_EAST:
            case DIR_WEST:
            body[0].setDesign(SNAKE_BODY_HORIZ);
            break;
            
            default:
            break;
        }

        if(body.size() > 1){
            // Iterate through body and update positions
            for(uint16_t n = 0; n < body.size(); n++){
                body[n+1] = body[n]; 
            }
        }
}

    // Move the head based on direction of the head
    moveHead();


}

void snake::moveHead(void){

    switch(direction){
        case DIR_NORTH:
        headPos.y -= 1;
        design = SNAKE_HEAD_NORTH;
        break;
        case DIR_SOUTH:
        headPos.y += 1;
        design = SNAKE_HEAD_SOUTH;
        break;
        case DIR_EAST:
        headPos.x += 1;
        design = SNAKE_HEAD_EAST;
        break;
        case DIR_WEST:
        headPos.x -= 1;
        design = SNAKE_HEAD_WEST;
        break;
        default:
        break;
    }

}

char snake::getDesign(void){
    return design;
}


const std::vector<snakeBodyElement>& snake::getBody(void){
    return body;
} 


snakeBodyElement::snakeBodyElement(s_pos position, char design): pos(position), design(design){

}

void snakeBodyElement::setPos(s_pos position){
    pos = position;
}

s_pos snakeBodyElement::getPos(void){
    return pos;
}

void snakeBodyElement::setDesign(char c){
    design = c;
}

char snakeBodyElement::getDesign(void){
    return design;
}