// Tracks state of game, instantiates other classes and handles printing to terminal

#include "game.h"
#include "common.h"
#include <random> // For random number generation facilities


snakeGame::snakeGame(){

// Setup catching keyboard inputs (wasd, space)

// Setup catching signals (SIGINT | SIGKILL | SIGTERM)

// Setup catching signal (SIGWINCH)

}

    
void snakeGame::run(){

    // Main loop

    // Set the position of the snake
    s_size termSize = getFrameSize();
    s_pos pos;
    pos.y = termSize.rows/2;
    pos.x = termSize.cols/2;
    sneakySnake.setHeadPos(pos);
    sneakySnake.setDirection(DIR_NORTH);

    // Start by placing food
    placeFood();

    while(1){

    // Check if food is consumed
    if(true == foodConsumed()){
        // If yes, place new food and extend the snake body
        sneakySnake.extendSnake();
        placeFood();
    }

    clearFrame();

    updateFrameLayout();

    // Print frame to terminal
    printFrame();

    // get latest command
    // use command to set next position of snake and design
    updateSnake('w');


    std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Pause for a short time
    }
}
    
void snakeGame::updateFrameLayout(void){
    // Update the frame with new positions    
    updateFrameElement(SNAKE_FOOD, foodPos.y, foodPos.x);

    s_pos pos = sneakySnake.getHeadPos();
    updateFrameElement(sneakySnake.getDesign(), pos.y, pos.x);

    std::vector<snakeBodyElement> snakeBody = sneakySnake.getBody();
    for(snakeBodyElement element: snakeBody){
        pos = element.getPos();
        updateFrameElement(element.getDesign(), pos.y, pos.x);
    }
}

// void snakeGame::pause(){

// }

// void snakeGame::exit(){

// }

bool snakeGame::foodConsumed(void){
    
    s_pos headPos = sneakySnake.getHeadPos();
    if((headPos.x == foodPos.x) && (headPos.y == foodPos.y)){
        return true;
    }

    return false;
}

void snakeGame::placeFood(void){

    s_size frameLimit = getFrameSize();

    std::random_device rd; // Obtain a random number from hardware
    std::mt19937 gen(rd()); // Seed the generator

    // For columns
    std::uniform_int_distribution<> distrib0(0, frameLimit.cols); // Define the distribution
    foodPos.x = distrib0(gen); // Generate the random number

    // For rows
    std::uniform_int_distribution<> distrib1(0, frameLimit.rows); // Define the distribution
    foodPos.y = distrib1(gen); // Generate the random number


    // foodPos = sneakySnake.getHeadPos();
    // foodPos.y -=5;
}

void snakeGame::updateSnake(char cmd){

    e_DIR dir = sneakySnake.getDirection();
    // update direction based on command
    switch(cmd){
        case 'w':
        case 'W':
            if((dir == DIR_EAST)||(dir == DIR_WEST)){
                sneakySnake.setDirection(DIR_NORTH);
            }
        break;
        
        case 'd':
        case 'D':
            if((dir == DIR_NORTH)||(dir == DIR_SOUTH)){
                sneakySnake.setDirection(DIR_WEST);
            }

        break;
        
        case 'a':
        case 'A':
            if((dir == DIR_NORTH)||(dir == DIR_SOUTH)){
                sneakySnake.setDirection(DIR_WEST);
            }
        break;
        
        case 's':
        case 'S':
            if((dir == DIR_EAST)||(dir == DIR_WEST)){
                sneakySnake.setDirection(DIR_SOUTH);
            }
        break;
        
        default:
        break;
    }

    // move snake by 1
    sneakySnake.moveSnake();

}
